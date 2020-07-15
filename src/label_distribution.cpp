#include <rf/label_distribution.h>

namespace rf {

LabelDistribution& LabelDistribution::combine(LabelDistribution const& other) {
  for (const auto& p : other) {
    dist_[p.first] += p.second;
  }

  double sum = 0.0;
  std::for_each(dist_.begin(), dist_.end(),
                [&sum](auto const& p) { sum += p.second; });

  std::for_each(dist_.begin(), dist_.end(),
                [&sum](auto& p) { p.second /= sum; });

  return *this;
}

}  // namespace rf
