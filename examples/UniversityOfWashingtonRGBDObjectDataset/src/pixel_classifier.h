#pragma once

#include <rf/core/split_candidate.h>

#include <random>

#include "image.h"

using PixelOffset = std::pair<int, int>;

class PixelClassifier
    : public rf::SplitCandidate<PixelClassifier, PixelReference> {
 public:
  rf::SplitResult classify(PixelReference const& p) const noexcept {
    // TODO to implement
    using PixelCoord = std::pair<int, int>;
    PixelCoord center = std::make_pair(p.row(), p.col());
    PixelCoord offset1 =
        std::make_pair(p.row() + o1_.first, p.col() + o1_.second);
    PixelCoord offset2 =
        std::make_pair(p.row() + o2_.first, p.col() + o2_.second);

    auto const& image = p.image();
    const auto centerDepth = image.getDepthValue(center.first, center.second);
    const auto offset1Depth =
        image.getDepthValue(offset1.first, offset1.second);
    const auto offset2Depth =
        image.getDepthValue(offset2.first, offset2.second);

    const auto ratio =
        (offset1Depth - centerDepth) / (offset2Depth - centerDepth);

    return ratio < t_ ? rf::SplitResult::LEFT : rf::SplitResult::RIGHT;
  }

  static PixelClassifier generate() {
    std::random_device rd{};
    std::mt19937 gen{rd()};

    // notice the images in this set are about ~80x80 pixels
    // here we choose a standard deviations of just half of that
    // this can be better thought
    std::normal_distribution<double> offsetDist{0, 40};
    std::normal_distribution<double> threshDist{1.0, 0.25};

    auto offset1 = std::make_pair(std::round(offsetDist(gen)),
                                  std::round(offsetDist(gen)));
    auto offset2 = std::make_pair(std::round(offsetDist(gen)),
                                  std::round(offsetDist(gen)));

    return PixelClassifier(offset1, offset2, threshDist(gen));
  }

  PixelClassifier() = default;
  explicit PixelClassifier(PixelOffset o1, PixelOffset o2, double t) noexcept
      : o1_{o1}, o2_{o2}, t_{t} {}

 private:
  PixelOffset o1_{};
  PixelOffset o2_{};
  double t_{0.0};
};
