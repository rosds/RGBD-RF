#pragma once

#include <rf/label.h>

#include <unordered_map>

namespace rf {

/**
 *  This class models a discrete distributions of labels. It is essentially a
 *  list of pairs of labels with their probability
 */
class LabelDistribution {
 public:
  LabelDistribution() = default;

  /**
   *  Convenient constructor to get a Label distribution from a sequence of
   *  labels
   */
  template <typename InputIterator>
  LabelDistribution(InputIterator begin, InputIterator end);

  /**
   *  Combine label distributions is performed to merge the votes from every
   *  tree in the forest
   */
  LabelDistribution& combine(LabelDistribution const& other);

  /**
   *  Compute and return the entropy level for the distribution.
   */
  [[nodiscard]] double entropy() const noexcept;

  /**
   *  Return the <label, prob> pair with max probability.
   */
  [[nodiscard]] std::pair<Label, double> maxProb() const noexcept;

 private:
  void updateMaxProb() noexcept;
  std::pair<Label, double> maxProb_{0, 0.0};
  std::unordered_map<Label, double> dist_{};
};

}  // namespace rf

#include "impl/label_distribution.hpp"
