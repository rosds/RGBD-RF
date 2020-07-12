#include "image.h"

#include <algorithm>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <random>

class ColorImageHandler : public ImageHandler {
 public:
  ColorImageHandler(fs::path const& path) noexcept : ImageHandler(path) {}

 public:  // ImageHandle
  void load() override {
    if (!loaded_) {
      img_ =
          cv::imread(path_.string(), cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);
      loaded_ = true;
    }
  }
  void release() override {
    if (loaded_) {
      img_.release();
      loaded_ = false;
    }
  }

  int rows() const noexcept override { return img_.rows; }
  int cols() const noexcept override { return img_.cols; }

 private:
  bool loaded_{false};
  cv::Mat img_{};
};

class DepthImageHandler : public ImageHandler {
 public:
  DepthImageHandler(fs::path const& path) noexcept : ImageHandler(path) {}

  [[nodiscard]] double getValue(int row, int col) const noexcept {
    if (loaded_ && row >= 0 && row < img_.rows && col >= 0 && col < img_.cols) {
      return img_.at<float>(row, col);
    }
    return 0.0;
  }

 public:  // ImageHandle
  void load() override {
    if (!loaded_) {
      img_ =
          cv::imread(path_.string(), cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);
      img_.convertTo(img_, CV_32F);
      loaded_ = true;
    }
  }
  void release() override {
    if (loaded_) {
      img_.release();
      loaded_ = false;
    }
  }

  int rows() const noexcept override { return img_.rows; }
  int cols() const noexcept override { return img_.cols; }

 private:
  bool loaded_{false};
  cv::Mat img_{};
};

/**
 *
 *  This class stores the (row, col) coordinates for those pixels that have
 *  value. The reason is to enable sampling from only these pixels.
 *
 */
class LabelImageHandler : public ImageHandler {
 public:
  LabelImageHandler(fs::path const& path) : ImageHandler(path) {}

  bool isLabeled(int row, int col) const noexcept {
    if (loaded_ && row >= 0 && row < img_.rows && col >= 0 && col < img_.cols) {
      const auto& pixel = img_.at<cv::Vec3b>(row, col);
      return cv::norm(pixel) > 0.0;
    }

    return false;
  }

 public:  // ImageHandle
  void load() override {
    if (!loaded_) {
      img_ =
          cv::imread(path_.string(), cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);

      // the label image should be a
      if (img_.type() != CV_8UC1) {
        throw std::runtime_error("unexpected label image type");
      }
      loaded_ = true;
    }
  }

  void release() override {
    if (loaded_) {
      img_.release();
      loaded_ = false;
    }
  }

  int rows() const noexcept { return img_.rows; }
  int cols() const noexcept { return img_.cols; }

 private:
  bool loaded_{false};
  cv::Mat img_{};
};

Image::Image(fs::path const& color, fs::path const& depth)
    : color_(new ColorImageHandler(color)),
      depth_(new DepthImageHandler(depth)) {}
Image::~Image() {}

void Image::load() {
  color_->load();
  depth_->load();
}

void Image::release() {
  color_->release();
  depth_->release();
}

int Image::rows() const noexcept { return color_->rows(); }
int Image::cols() const noexcept { return color_->cols(); }

double Image::getDepthValue(int row, int col) const noexcept {
  auto img = dynamic_cast<DepthImageHandler*>(depth_.get());
  return img->getValue(row, col);
}

LabeledImage::LabeledImage(fs::path const& color, fs::path const& depth,
                           fs::path const& labels, rf::Label label,
                           rf::Label defaultLabel)
    : labels_{new LabelImageHandler(labels)},
      Image(color, depth),
      label_{label},
      bgLabel_{defaultLabel} {}
LabeledImage::~LabeledImage() {}

void LabeledImage::load() {
  Image::load();
  labels_->load();
}

void LabeledImage::release() {
  Image::release();
  labels_->release();
}

rf::Label LabeledImage::getLabelValue(int row, int col) const noexcept {
  auto image = dynamic_cast<LabelImageHandler*>(labels_.get());
  if (image->isLabeled(row, col)) {
    return label_;
  } else {
    return bgLabel_;
  }
}

std::vector<PixelReference> sampleLabeledPixels(LabeledImage const& image,
                                                size_t n) {
  std::vector<PixelReference> samples;
  auto gen = std::mt19937(std::random_device{}());
  auto rowDist = std::uniform_int_distribution<int>{0, image.rows()};
  auto colDist = std::uniform_int_distribution<int>{0, image.cols()};

  for (size_t i = 0; i < n; ++i) {
    const auto row = rowDist(gen);
    const auto col = colDist(gen);
    samples.emplace_back(PixelReference(image, rowDist(gen), colDist(gen)));
  }

  return samples;
}
