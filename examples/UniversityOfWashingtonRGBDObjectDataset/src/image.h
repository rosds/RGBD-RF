#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "rf/core/label.h"

namespace fs = std::filesystem;

class ImageHandler {
 public:
  ImageHandler(fs::path const& path) noexcept : path_{path} {}
  virtual ~ImageHandler() = default;

 public:
  virtual void load() = 0;
  virtual void release() = 0;

 protected:
  fs::path path_{};
};

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
  Image(fs::path const& color, fs::path const& depth);
  ~Image();
  Image(Image&&) = default;
  Image& operator=(Image&&) = default;

  virtual void load();
  virtual void release();

 protected:
  std::unique_ptr<ImageHandler> color_{nullptr};
  std::unique_ptr<ImageHandler> depth_{nullptr};
};

/**
 *
 *  This is the object to be classified by the tree. It his represented as a
 *  lightweight reference to a image's pixel.
 *
 */
class PixelReference {
 public:
  PixelReference(Image const& image, ssize_t row, ssize_t col) noexcept
      : row_{row}, col_{col}, image_{image} {}

 private:
  ssize_t row_{0};
  ssize_t col_{0};
  std::reference_wrapper<const Image> image_;
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
  LabeledImage(fs::path const& color, fs::path const& depth,
               fs::path const& labels);
  ~LabeledImage();
  LabeledImage(LabeledImage&&) = default;
  LabeledImage& operator=(LabeledImage&&) = default;

  void load() override;
  void release() override;

 protected:
  friend std::vector<PixelReference> sampleLabeledPixels(
      LabeledImage const& image, size_t n);

  std::unique_ptr<ImageHandler> labels_{nullptr};
};

/**
 *
 *  Sample a labeled pixel out of the image
 *
 */
std::vector<PixelReference> sampleLabeledPixels(LabeledImage const& image,
                                                size_t n);
