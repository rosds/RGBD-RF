#pragma once

#include <rf/core/label.h>
#include <rf/core/train_set.h>

#include <unordered_map>
#include <vector>

#include "image.h"

class TrainingSet;

/**
 *
 *  Keeps a reference to all the images in the file system for a particular
 *  label.
 *
 *  Movable only.
 *
 */
class ImagePool {
 public:
  using ImageList = std::vector<LabeledImage>;
  using ImageIterator = ImageList::iterator;

  ImagePool() = default;
  ImagePool(ImagePool const&) = delete;
  ImagePool& operator=(ImagePool const&) = delete;

  // Append more images to the pool
  void append(ImageList&& images);

  // shuffle the images
  void shuffle();

  [[nodiscard]] size_t size() const noexcept { return pool_.size(); }

 private:
  friend std::tuple<TrainingSet, TrainingSet, TrainingSet> splitImagePool(
      ImagePool& pool, double validationSize, double testSize);

  ImageList pool_{};
};

using TrainingExample = rf::TrainingExample<PixelReference>;

/**
 *
 *  This class will refer to the image pool and act as a sort of iterator.
 *
 */
class TrainingSet : public rf::TrainSet<PixelReference> {
 public:
  using ImageIterator = ImagePool::ImageIterator;

  TrainingSet(ImageIterator begin, ImageIterator end) noexcept
      : begin_{begin}, end_{end} {}
  ~TrainingSet();

  size_t size() const noexcept;
  void setSamplesPerClass(size_t n) { samplesPerClass_ = n; }
  void setSamplesPerImage(size_t n) { samplesPerImage_ = n; }

 public:  // TrainSet
  std::vector<TrainingExample> sample() override;
  std::unique_ptr<TrainSetIterator> iter() override;

  [[nodiscard]] ImageIterator begin() const noexcept { return begin_; }
  [[nodiscard]] ImageIterator end() const noexcept { return end_; }

 private:
  size_t samplesPerImage_{0};
  size_t samplesPerClass_{0};
  ImageIterator begin_{};
  ImageIterator end_{};
  std::vector<std::reference_wrapper<LabeledImage>> loadedImages_{};
};

/**
 *
 *  Split the image pool into train, validation and test sets.
 *
 *  \param validationSize float between 0 and 1.
 *  \param testSize float betweeen 0 and 1.
 */
std::tuple<TrainingSet, TrainingSet, TrainingSet> splitImagePool(
    ImagePool& pool, double validationSize, double testSize);
