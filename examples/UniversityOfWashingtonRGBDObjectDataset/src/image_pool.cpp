#include "image_pool.h"

#include <algorithm>
#include <random>

using ImageIterator = ImagePool::ImageIterator;

void ImagePool::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());

  for (auto& entry : pool_) {
    auto& images = entry.second;
    std::shuffle(images.begin(), images.end(), g);
  }
}

size_t ImagePool::size() const noexcept {
  size_t count = 0;
  for (auto const& cls : pool_) {
    count += cls.second.size();
  }
  return count;
}

size_t TrainingSet::size() const noexcept {
  size_t count = 0;
  for (auto const& range : classRanges_) {
    count += std::distance(range.first, range.second);
  }
  return count;
}

std::tuple<TrainingSet, TrainingSet, TrainingSet> splitImagePool(
    ImagePool const& imagePool, double validationSize, double testSize) {
  // check validationSize and testSize are acceptable
  auto isValid = [](double d) { return d >= 0.0 && d <= 1.0; };
  if (!isValid(validationSize) || !isValid(testSize) ||
      validationSize + testSize >= 1.0) {
    throw std::logic_error("invalid validation and test sets sizes");
  }

  double const trainSize = 1.0 - validationSize - testSize;
  auto const& pool = imagePool.pool_;

  TrainingSet::ImageRanges trainPairs{};
  TrainingSet::ImageRanges validationPairs{};
  TrainingSet::ImageRanges testPairs{};

  // take
  for (auto const& cls : pool) {
    // images of this class
    const auto& images = cls.second;
    const size_t imagesCount = images.size();

    // Number of training images to take
    const size_t cut1 = static_cast<size_t>(imagesCount * trainSize);
    const size_t cut2 =
        cut1 + static_cast<size_t>(imagesCount * validationSize);

    auto it = images.cbegin();
    trainPairs.emplace_back(it, it + cut1);
    validationPairs.emplace_back(it + cut1, it + cut2);
    testPairs.emplace_back(it + cut2, images.cend());
  }

  return {TrainingSet(std::move(trainPairs)),
          TrainingSet(std::move(validationPairs)),
          TrainingSet(std::move(testPairs))};
}
