#include <cassert>
#include <filesystem>
#include <iostream>
#include <vector>

#include "image_pool.h"
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

std::vector<LabeledImage> findImages(fs::path const& directory,
                                     std::string_view color_suffix,
                                     std::string_view depth_suffix,
                                     std::string_view label_suffix) {
  auto images = std::vector<LabeledImage>{};

  for (auto file : fs::directory_iterator(directory)) {
    if (file.is_directory()) {
      // append the sub-folder images
      auto sub = findImages(file, color_suffix, depth_suffix, label_suffix);
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
        images.emplace_back(file, depth_file, label_file);
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

  // Image Pool
  ImagePool pool{};

  // gather the images for each class
  const auto& classes = node["classes"];
  for (auto it = classes.begin(); it != classes.end(); ++it) {
    const auto label = it->first.as<std::string>();
    const auto directory = fs::path{it->second.as<std::string>()};
    auto images =
        findImages(directory, color_suffix, depth_suffix, label_suffix);

    pool.addObjectType(label, std::move(images));
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
  train.setSamplesPerClass(20);
  train.setSamplesPerImage(50);

  return 0;
}
