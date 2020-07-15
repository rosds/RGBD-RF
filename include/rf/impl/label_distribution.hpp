#include <rf/label_distribution.h>

namespace rf {

/**
 *  Compute the label distribution
 */
template <typename InputIterator>
LabelDistribution::LabelDistribution(InputIterator begin, InputIterator end) {
  double total = 0.0;
  for (auto it = begin; it != end; ++it) {
    const auto label = static_cast<Label>(it->second);
    dist_[label] += 1.0;
    total += 1.0;
  }

  for (auto& cls : dist_) {
    cls.second /= total;
  }
}

}  // namespace rf
