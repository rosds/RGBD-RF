#pragma once

#include <unordered_map>
#include <vector>

#include "image.h"
#include "rf/core/label.h"

// Define the Label Type
struct Object {};
using ObjectLabel = rf::Label<Object>;

class ImagePool {
 public:
  ImagePool() = default;
  ImagePool(ImagePool const&) = delete;
  ImagePool& operator=(ImagePool const&) = delete;

  void addObjectType(ObjectLabel label, std::vector<LabeledImage>&& images) {
    pool_.emplace(label, std::move(images));
  }

 private:
  std::unordered_map<ObjectLabel::underlying_type, std::vector<LabeledImage>>
      pool_;
};
