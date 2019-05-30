#ifndef TRAIN_FOREST_IMAGE_INFO_H
#define TRAIN_FOREST_IMAGE_INFO_H

#include <iostream>
#include <string>

namespace train_forest {
struct ImageInfo {
    std::string name;
    std::string source;
    std::string labels;

    friend std::ostream& operator<<(std::ostream& os, ImageInfo const& img);
};

std::ostream& operator<<(std::ostream& os, ImageInfo const& img) {
    os << "ImageInfo{ name: " << img.name << " }";
    return os;
}

}  // namespace train_forest

#endif  // TRAIN_FOREST_IMAGE_INFO_H
