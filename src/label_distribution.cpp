#include <rf/label_distribution.h>

#include <cassert>
#include <cmath>

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

double LabelDistribution::entropy() const noexcept {
  double entropy = 0.0;
  for (auto const& entry : dist_) {
    auto p = entry.second;
    assert(p >= 0.0 && p <= 1.0);
    entropy += -p * std::log(p);
  }
  return entropy;
}

}  // namespace rf
