#pragma once
#include <filesystem>

#include "rf/core/label.h"

namespace fs = std::filesystem;

/**
 *
 *  This is the class used to hold the information to be classified.
 *
 *  In the case of this example, it holds the RGB color image together with the
 *  depth information.
 *
 */
class Image {
 public:
  explicit Image(fs::path const& color, fs::path const& depth) noexcept
      : color_(color), depth_(depth) {}

 private:
  fs::path color_;
  fs::path depth_;
};

/**
 *
 *  Inherits from Image for convenience to cast to Image.
 *
 *  Additionally store the label information.
 *
 */
class LabeledImage : public Image {
 public:
  explicit LabeledImage(fs::path const& color, fs::path const& depth,
                        fs::path const& labels) noexcept
      : labels_(labels), Image(color, depth) {}

 private:
  fs::path labels_;
};
