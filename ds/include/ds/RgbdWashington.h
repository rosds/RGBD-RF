#ifndef DATASET_READER_RGBD_WASHINGTON_H
#define DATASET_READER_RGBD_WASHINGTON_H

#include <rf/LabelImage.h>
#include <rf/TrainSet.h>
#include <rf/TrainingImage.h>

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

/**
 * https://rgbd-dataset.cs.washington.edu/index.html
 */
class RgbdWashingtonDataset {
   public:
    using DepthImage = rf::Image<float>;
    using LabelImage = rf::LabelImage<rf::Image<uint8_t>>;
    using TrainingImage = rf::TrainingImage<DepthImage, LabelImage>;
    using TrainSet = rf::TrainSet<TrainingImage>;

    RgbdWashingtonDataset() = default;

    void emplace_image_directory(fs::path directory);

    TrainSet load() const;

   private:
    std::vector<fs::path> directories_{};
};

#endif  // DATASET_READER_RGBD_WASHINGTON_H
