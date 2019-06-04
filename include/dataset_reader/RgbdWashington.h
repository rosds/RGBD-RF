#ifndef DATASET_READER_RGBD_WASHINGTON_H
#define DATASET_READER_RGBD_WASHINGTON_H

#include "ImageFileLoader.hpp"

#include <rf/LabelImage.h>
#include <rf/TrainingImage.h>

#include <boost/filesystem.hpp>
#include <regex>
#include <sstream>
#include <vector>

namespace fs = boost::filesystem;

/**
 * https://rgbd-dataset.cs.washington.edu/index.html
 */
class RgbdWashingtonDataset {
   public:
    using DepthImage = rf::Image<float>;
    using LabelImage = rf::LabelImage<rf::Image<uint8_t>>;
    using TrainingImage = rf::TrainingImage<DepthImage, LabelImage>;

    RgbdWashingtonDataset() = default;

    void emplace_image_directory(fs::path directory) {
        if (fs::is_directory(directory)) {
            directories_.emplace_back(directory);
        } else {
            std::ostringstream error;
            error << "Not a directory: " << directory;
            throw std::runtime_error(error.str());
        }
    }

    std::vector<TrainingImage> load() const {
        auto images = std::vector<TrainingImage>{};
        const std::regex depth_image_filename{"(\\w+)_depthcrop.png"};
        for (const auto& directory : directories_) {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (!fs::is_regular(entry)) {
                    continue;
                }

                const auto filename = entry.path().filename().string();
                std::smatch filename_match;
                if (std::regex_match(filename, filename_match,
                                     depth_image_filename)) {
                    // labels image
                    const auto label_image =
                        directory /
                        fs::path{filename_match[1].str() + "_maskcrop.png"};

                    auto depth = ImageFileLoader::Load<float>(entry.path());
                    auto label =
                        LabelImage{ImageFileLoader::Load<uint8_t>(label_image)};

                    images.emplace_back(std::move(depth), std::move(label));
                }
            }
        }
        return images;
    }

   private:
    std::vector<fs::path> directories_{};
};

#endif  // DATASET_READER_RGBD_WASHINGTON_H
