#pragma once

#include <unordered_map>
#include <vector>

#include "image.h"
#include "rf/core/label.h"
#include "rf/core/train_set.h"

using LabelType = rf::StringLabelMap::LabelType;

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
  using ImageIterator = std::vector<LabeledImage>::iterator;

  ImagePool() = default;
  ImagePool(ImagePool const&) = delete;
  ImagePool& operator=(ImagePool const&) = delete;

  void addObjectType(std::string label, std::vector<LabeledImage>&& images) {
    pool_.emplace(std::move(label), std::move(images));
  }

  // Just shuffle the images
  void shuffle();

  // Return the total of images in the pool
  size_t size() const noexcept;

 private:
  friend std::tuple<TrainingSet, TrainingSet, TrainingSet> splitImagePool(
      ImagePool& pool, double validationSize, double testSize);

  std::unordered_map<std::string, std::vector<LabeledImage>> pool_{};
};

/**
 *
 *  This class will refer to the image pool and act as a sort of iterator.
 *
 */
class TrainingSet : public rf::TrainSet<PixelReference, LabelType> {
  using rf::TrainSet<PixelReference, LabelType>::TrainingExample;
  using ImageIterator = ImagePool::ImageIterator;

 public:
  using ImageRanges =
      std::vector<std::tuple<std::string, ImageIterator, ImageIterator>>;

  TrainingSet(ImageRanges&& ls) noexcept : classRanges_{ls} {};

  size_t size() const noexcept;
  void setSamplesPerClass(size_t n) { samplesPerClass_ = n; }
  void setSamplesPerImage(size_t n) { samplesPerImage_ = n; }

 public:  // TrainSet
  std::vector<TrainingExample> sample() override;

 private:
  size_t samplesPerImage_{0};
  size_t samplesPerClass_{0};
  ImageRanges classRanges_{};
  rf::StringLabelMap labels_{};
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
