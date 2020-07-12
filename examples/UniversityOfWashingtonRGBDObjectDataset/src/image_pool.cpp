#include "image_pool.h"

#include <algorithm>
#include <random>

using ImageIterator = ImagePool::ImageIterator;

void ImagePool::append(std::vector<LabeledImage>&& images) {
  pool_.insert(pool_.end(), std::make_move_iterator(images.begin()),
               std::make_move_iterator(images.end()));
}

void ImagePool::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(pool_.begin(), pool_.end(), g);
}

size_t TrainingSet::size() const noexcept {
  return std::distance(begin_, end_);
}

std::vector<TrainingExample> TrainingSet::sample() {
  std::vector<TrainingExample> samples;

  // Release the images
  for (auto& img : loadedImages_) {
    img.get().release();
  }

  loadedImages_.clear();
  std::sample(begin_, end_, std::back_inserter(loadedImages_), samplesPerClass_,
              std::mt19937{std::random_device{}()});

  // load the images
  for (auto& img : loadedImages_) {
    img.get().load();
  }

  for (const auto& image : loadedImages_) {
    auto pixels = sampleLabeledPixels(image, samplesPerImage_);
    std::transform(pixels.begin(), pixels.end(), std::back_inserter(samples),
                   [&](const auto& pixelReference) {
                     const auto label = image.get().getLabelValue(
                         pixelReference.row(), pixelReference.col());
                     return std::make_pair(pixelReference, label);
                   });
  }

  return samples;
}

class TrainSetIteratorImpl
    : public rf::TrainSet<PixelReference>::TrainSetIterator {
 public:
  TrainSetIteratorImpl(TrainingSet const& set) noexcept
      : set_{set}, it_{set.begin()} {}

 public:  // rf::TrainSet::TrainSetIterator
  void next() override {
    if (it_ == set_.get().end()) {
      return;
    }

    if (pixel_ == 0) {
      it_->load();
    }

    int rows = it_->rows();
    int cols = it_->cols();
    size_t pixelCount = rows * cols;

    if (pixel_ < pixelCount) {
      pixel_++;
    } else {
      pixel_ = 0;
      it_->release();
      it_++;
    }
  }

  std::optional<TrainingExample> value() override {
    if (it_ == set_.get().end()) {
      return std::nullopt;
    }

    it_->load();

    int row = pixel_ / it_->rows();
    int col = pixel_ % it_->cols();
    return std::make_pair(PixelReference(*it_, row, col),
                          it_->getLabelValue(row, col));
  }

 private:
  size_t pixel_{0};
  ImageIterator it_{};
  std::reference_wrapper<const TrainingSet> set_;
};

std::unique_ptr<TrainingSet::TrainSetIterator> TrainingSet::iter() {
  return std::make_unique<TrainSetIteratorImpl>(*this);
}

TrainingSet::~TrainingSet() {}

std::tuple<TrainingSet, TrainingSet, TrainingSet> splitImagePool(
    ImagePool& imagePool, double validationSize, double testSize) {
  // check validationSize and testSize are acceptable
  auto isValid = [](double d) { return d >= 0.0 && d <= 1.0; };
  if (!isValid(validationSize) || !isValid(testSize) ||
      validationSize + testSize >= 1.0) {
    throw std::logic_error("invalid validation and test sets sizes");
  }

  double const trainSize = 1.0 - validationSize - testSize;
  auto& pool = imagePool.pool_;
  size_t const imagePoolSize = pool.size();

  // Number of training images to take
  const auto trainIt = pool.begin();
  const auto validIt =
      pool.begin() + static_cast<size_t>(imagePoolSize * trainSize);
  const auto testIt =
      validIt + static_cast<size_t>(imagePoolSize * validationSize);

  return {{trainIt, validIt}, {validIt, testIt}, {testIt, pool.end()}};
}
