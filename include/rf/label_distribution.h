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

  auto begin() const noexcept { return dist_.begin(); }
  auto end() const noexcept { return dist_.end(); }

  /**
   *  Combine label distributions is performed to merge the votes from every
   *  tree in the forest
   */
  LabelDistribution& combine(LabelDistribution const& other);

  /**
   *  Compute and return the entropy level for the distribution.
   */
  [[nodiscard]] double entropy() const noexcept;

 private:
  std::unordered_map<Label, double> dist_{};
};

}  // namespace rf

#include "impl/label_distribution.hpp"
