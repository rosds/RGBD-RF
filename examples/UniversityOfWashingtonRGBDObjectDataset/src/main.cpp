#include <rf/core/tree.h>

#include <cassert>
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <vector>

#include "image_pool.h"
#include "pixel_classifier.h"
#include "yaml-cpp/yaml.h"

namespace fs = std::filesystem;

bool hasSuffix(std::string_view string, std::string_view suffix) {
  return string.compare(string.length() - suffix.length(), suffix.length(),
                        suffix) == 0;
}

std::string replaceSuffix(std::string_view string, std::string_view suffix,
                          std::string_view newSuffix) {
  auto copy = std::string{string};
  copy.replace(string.length() - suffix.length(), suffix.length(), newSuffix);
  return copy;
}

std::vector<LabeledImage> findImages(fs::path const& directory, rf::Label label,
                                     rf::Label bgLabel,
                                     std::string_view color_suffix,
                                     std::string_view depth_suffix,
                                     std::string_view label_suffix) {
  auto images = std::vector<LabeledImage>{};

  for (auto file : fs::directory_iterator(directory)) {
    if (file.is_directory()) {
      // append the sub-folder images
      auto sub = findImages(file, label, bgLabel, color_suffix, depth_suffix,
                            label_suffix);
      images.insert(images.end(), std::make_move_iterator(sub.begin()),
                    std::make_move_iterator(sub.end()));
    } else {
      const auto& filename = file.path().filename().string();
      if (hasSuffix(filename, color_suffix)) {
        auto depth_file = file.path().parent_path() /
                          replaceSuffix(filename, color_suffix, depth_suffix);
        auto label_file = file.path().parent_path() /
                          replaceSuffix(filename, color_suffix, label_suffix);

        assert(fs::exists(depth_file) && fs::exists(label_file));
        images.emplace_back(file, depth_file, label_file, label, bgLabel);
      }
    }
  }

  return images;
}

/***
 *  The YAML configuration file for this example should specify
 *
 *
 *  Example configuration:
 *
 *  images:
 *    color_suffix: _crop.png
 *    depth_suffix: _depthcrop.png
 *    label_suffix: _maskcrop.png
 *
 *  classes:
 *    apple: /path/to/apple/images
 *    banana: /path/to/banana/images
 *
 *
 */
int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " configuration.yaml\n";
    return 1;
  }

  auto node = YAML::LoadFile(argv[1]);

  const auto color_suffix = node["color_suffix"].as<std::string>();
  const auto depth_suffix = node["depth_suffix"].as<std::string>();
  const auto label_suffix = node["label_suffix"].as<std::string>();

  // Map from strings to rf::Label
  auto& labelRegistry = rf::LabelRegistry<std::string>::instance();
  const auto bgLabel =
      rf::LabelRegistry<std::string>::instance().getLabel("background");

  // Image Pool
  ImagePool pool{};

  // gather the images for each class
  const auto& classes = node["classes"];
  for (auto it = classes.begin(); it != classes.end(); ++it) {
    const auto label = it->first.as<std::string>();
    const auto directory = fs::path{it->second.as<std::string>()};
    auto images = findImages(directory, labelRegistry.getLabel(label), bgLabel,
                             color_suffix, depth_suffix, label_suffix);

    pool.append(std::move(images));
  }

  // Shuffle
  pool.shuffle();

  // We split the dataset into train, validation and test.
  auto [train, validation, test] = splitImagePool(pool, 0.2, 0.2);

  std::cout << "Total Images:      " << pool.size() << "\n\n";
  std::cout << "Training Images:   " << train.size() << '\n';
  std::cout << "Validation Images: " << validation.size() << '\n';
  std::cout << "Test images:       " << test.size() << '\n';

  // Each iteration of the algorithm it will sample 20 x 50 pixels
  train.setSamplesPerClass(1000);
  train.setSamplesPerImage(80);

  rf::TreeParameters stoppingCriteria;
  stoppingCriteria.minSamplesPerNode = 20;
  stoppingCriteria.maxDepth = 10;
  stoppingCriteria.candidatesToGeneratePerNode = 1000;
  auto tree =
      rf::trainTree<PixelClassifier>(train, validation, stoppingCriteria);

  cv::namedWindow("classified");

  /**
   *
   * uncomment this to display the classified images
   *
   */
  std::vector<cv::Vec3b> colors = {cv::Vec3b{0, 0, 0}, cv::Vec3b{0, 0, 250},
                                   cv::Vec3b{0, 250, 0}};
  for (auto& img : test) {
    img.load();

    // load the rgb image with opencv
    auto pic = cv::imread(img.getColorPath().string(),
                          cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);

    cv::cvtColor(pic, pic, cv::COLOR_RGB2GRAY);
    cv::cvtColor(pic, pic, cv::COLOR_GRAY2RGB);

    for (int row = 0; row < img.rows(); ++row) {
      for (int col = 0; col < img.cols(); ++col) {
        auto pixel = img.ref(row, col);
        auto dist = tree.classify(pixel);

        auto label = std::max_element(dist.begin(), dist.end(),
                                      [](auto const& a, auto const& b) {
                                        return a.second < b.second;
                                      })
                         ->first;

        if (label != 0) {
          pic.at<cv::Vec3b>(row, col) = colors[label];
        }
      }
    }

    cv::imshow("classified", pic);
    cv::waitKey(0);

    img.release();
  }

  // Classification rate
  std::cout << "\nTest classification rate: " << rf::evaluateTree(tree, test)
            << '\n';

  return 0;
}
