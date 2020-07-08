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

 private:
  bool loaded_{false};
  cv::Mat img_{};
};

class DepthImageHandler : public ImageHandler {
 public:
  DepthImageHandler(fs::path const& path) noexcept : ImageHandler(path) {}

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
struct LabelImageHandler : public ImageHandler {
  LabelImageHandler(fs::path const& path) : ImageHandler(path) {}

  using PixelCoordinates = std::pair<int, int>;
  std::vector<PixelCoordinates> pixels{};

 public:  // ImageHandle
  void load() override {
    const auto img =
        cv::imread(path_.string(), cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);

    // the label image should be a
    if (img.type() != CV_8UC3) {
      throw std::runtime_error("unexpected label image type");
    }

    for (int i = 0; i < img.rows; ++i) {
      for (int j = 0; j < img.cols; ++j) {
        const auto& pixel = img.at<cv::Vec3b>(i, j);
        if (cv::norm(pixel) > 0.0) {
          pixels.emplace_back(i, j);
        }
      }
    }
  }
  void release() override { pixels.clear(); }
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

LabeledImage::LabeledImage(fs::path const& color, fs::path const& depth,
                           fs::path const& labels)
    : labels_{new LabelImageHandler(labels)}, Image(color, depth) {}
LabeledImage::~LabeledImage() {}

void LabeledImage::load() {
  Image::load();
  labels_->load();
}

void LabeledImage::release() {
  Image::release();
  labels_->release();
}

using LabelType = rf::StringLabelMap::LabelType;

std::vector<PixelReference> sampleLabeledPixels(LabeledImage const& image,
                                                size_t n) {
  const auto& pixels =
      dynamic_cast<LabelImageHandler*>(image.labels_.get())->pixels;

  std::vector<PixelReference> samples;
  auto gen = std::mt19937(std::random_device{}());
  auto dist = std::uniform_int_distribution<size_t>{0UL, pixels.size() - 1};

  for (size_t i = 0; i < n; ++i) {
    const auto pixel = pixels.begin() + dist(gen);
    samples.emplace_back(PixelReference(image, pixel->first, pixel->second));
  }

  return samples;
}
