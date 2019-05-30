#ifndef TRAIN_FOREST_CONFIG_H
#define TRAIN_FOREST_CONFIG_H

#include <vector>

#include "ImageInfo.h"

namespace train_forest {

struct TrainForestConfig {
    std::vector<ImageInfo> images;
};

}  // namespace train_forest

#endif  // TRAIN_FOREST_CONFIG_H
