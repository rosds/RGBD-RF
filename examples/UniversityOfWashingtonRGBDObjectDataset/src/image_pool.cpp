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
    const auto begin = std::get<1>(range);
    const auto end = std::get<2>(range);
    count += std::distance(begin, end);
  }
  return count;
}

std::vector<TrainingSet::TrainingExample> TrainingSet::sample() {
  std::vector<TrainingExample> samples;

  const size_t classCount = classRanges_.size();
  for (const auto& range : classRanges_) {
    const auto label = labels_.toLabel(std::get<0>(range));
    const auto begin = std::get<1>(range);
    const auto end = std::get<2>(range);

    // Release the images
    for (auto& img : loadedImages_) {
      img.get().release();
    }

    loadedImages_.clear();
    std::sample(begin, end, std::back_inserter(loadedImages_), samplesPerClass_,
                std::mt19937{std::random_device{}()});

    // load the images
    for (auto& img : loadedImages_) {
      img.get().load();
    }

    for (const auto& image : loadedImages_) {
      auto pixels = sampleLabeledPixels(image, samplesPerImage_);
      std::transform(pixels.begin(), pixels.end(), std::back_inserter(samples),
                     [&label](const auto& pixelReference) {
                       return std::make_pair(pixelReference, label);
                     });
    }
  }

  return samples;
}

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

  TrainingSet::ImageRanges trainPairs{};
  TrainingSet::ImageRanges validationPairs{};
  TrainingSet::ImageRanges testPairs{};

  // take
  for (auto& cls : pool) {
    // images of this class
    auto label = cls.first;
    auto& images = cls.second;
    size_t imagesCount = images.size();

    // Number of training images to take
    const size_t cut1 = static_cast<size_t>(imagesCount * trainSize);
    const size_t cut2 =
        cut1 + static_cast<size_t>(imagesCount * validationSize);

    auto it = images.begin();

    validationPairs.emplace_back(label, it, it + cut1);
    validationPairs.emplace_back(label, it + cut1, it + cut2);
    testPairs.emplace_back(label, it + cut2, images.end());
  }

  return std::make_tuple(trainPairs, std::move(validationPairs),
                         std::move(testPairs));
}
