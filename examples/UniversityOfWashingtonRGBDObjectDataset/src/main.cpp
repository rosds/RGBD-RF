#include <rf/random_forest.h>

#include <cassert>
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <vector>

#include "image_pool.h"
#include "pixel_classifier.h"

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

int main(int argc, char* argv[]) {
  /**
   *  For this example we will only train on apples and bananas
   */
  auto imageFolders = std::vector<std::pair<std::string, fs::path>>{
      {"apple", "../dataset/rgbd-dataset/apple"},
      {"banana", "../dataset/rgbd-dataset/banana"},
  };

  /**
   *  In this dataset there are 3 images per take on an object:
   *
   *  - The RGB picture in a 3-channel unsigned 8-bit image
   *  - The Depth information in a 1-channel 32 float point image
   *  - The mask information in a 1-channel unsigned 8-bit image
   *
   *  these are the suffixes for each object picture:
   */
  constexpr auto color_suffix = "_crop.png";
  constexpr auto depth_suffix = "_depthcrop.png";
  constexpr auto label_suffix = "_maskcrop.png";

  // Register the "background" label
  auto& labelRegistry = rf::LabelRegistry<std::string>::instance();
  const auto bgLabel =
      rf::LabelRegistry<std::string>::instance().getLabel("background");

  // Image Pool
  ImagePool pool{};

  // gather the images for each class
  for (auto const& entry : imageFolders) {
    auto const& [label, directory] = entry;
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
  train.setSamplesPerClass(500);
  train.setSamplesPerImage(80);

  rf::TreeParameters params;
  params.numberOfTrees = 10;
  params.minSamplesPerNode = 20;
  params.maxDepth = 5;
  params.candidatesToGeneratePerNode = 1000;
  rf::RandomForest<PixelClassifier> forest{};
  forest.train(train, validation, params);

  cv::namedWindow("classified");

  std::vector<cv::Vec3b> colors = {
      cv::Vec3b{0, 0, 0},      // this one is for the background but is not used
      cv::Vec3b{0, 0, 255},    // color apples red
      cv::Vec3b{0, 255, 255},  // color bananas yellow
  };

  size_t idx = 0;
  // Show 20 classified images from the test set
  for (auto it = test.begin(); it != test.end() && idx < 20; ++it, ++idx) {
    auto& img = *it;
    img.load();

    // load the rgb image with opencv
    auto pic = cv::imread(img.getColorPath().string(),
                          cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);

    cv::cvtColor(pic, pic, cv::COLOR_RGB2GRAY);
    cv::cvtColor(pic, pic, cv::COLOR_GRAY2RGB);

    for (int row = 0; row < img.rows(); ++row) {
      for (int col = 0; col < img.cols(); ++col) {
        auto pixel = img.ref(row, col);
        auto dist = forest.classify(pixel);

        auto label = dist.maxProb().first;

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
  std::cout << "\nTest classification error: " << rf::evaluateTree(forest, test)
            << '\n';

  return 0;
}
