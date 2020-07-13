#pragma once

#include <rf/core/label.h>

#include <filesystem>
#include <memory>
#include <vector>

namespace fs = std::filesystem;

class ImageHandler {
 public:
  ImageHandler(fs::path const& path) noexcept : path_{path} {}
  virtual ~ImageHandler() = default;

 public:
  virtual void load() = 0;
  virtual void release() = 0;
  virtual int rows() const noexcept = 0;
  virtual int cols() const noexcept = 0;

  [[nodiscard]] fs::path path() const noexcept { return path_; }

 protected:
  fs::path path_{};
};

class PixelReference;

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

  int rows() const noexcept;
  int cols() const noexcept;

  [[nodiscard]] double getDepthValue(int row, int col) const noexcept;
  [[nodiscard]] fs::path getColorPath() const noexcept {
    return color_->path();
  }

  [[nodiscard]] PixelReference ref(int row, int col) const noexcept;

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
  PixelReference(Image const& image, int row, int col) noexcept
      : row_{row}, col_{col}, image_{image} {}

  [[nodiscard]] int row() const noexcept { return row_; }
  [[nodiscard]] int col() const noexcept { return col_; }
  [[nodiscard]] Image const& image() const noexcept { return image_.get(); }

 private:
  int row_{0};
  int col_{0};
  std::reference_wrapper<const Image> image_;
};

/**
 *
 *  Inherits from Image for convenience to cast to Image.
 *
 *  Notice that for this particular training set, each image has a single
 *  object therefore a single label. Also only a region of the images is
 *  occupied by the object, the rest of the image is classified as
 *  "background".
 *
 */
class LabeledImage : public Image {
 public:
  LabeledImage(fs::path const& color, fs::path const& depth,
               fs::path const& labels, rf::Label label, rf::Label bgLabel);

  ~LabeledImage();
  LabeledImage(LabeledImage&&) = default;
  LabeledImage& operator=(LabeledImage&&) = default;

  void load() override;
  void release() override;

  [[nodiscard]] rf::Label getLabelValue(int row, int col) const noexcept;

 protected:
  friend std::vector<PixelReference> sampleLabeledPixels(
      LabeledImage const& image, size_t n);

  rf::Label label_{};
  rf::Label bgLabel_{};
  std::unique_ptr<ImageHandler> labels_{nullptr};
};

/**
 *
 *  Sample a labeled pixel out of the image
 *
 */
std::vector<PixelReference> sampleLabeledPixels(LabeledImage const& image,
                                                size_t n);
