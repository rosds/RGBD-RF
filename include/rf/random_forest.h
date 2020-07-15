#pragma once

#include "tree.h"

namespace rf {

template <typename SplitCandidate>
class RandomForest {
  using Data = typename SplitCandidate::Input;

 public:
  RandomForest() = default;

  void train(TrainSet<Data>& train, TrainSet<Data>& validation,
             TreeParameters const& params) {
    forest_.clear();
    for (size_t i = 0; i < params.numberOfTrees; ++i) {
      forest_.emplace_back(
          trainTree<SplitCandidate>(train, validation, params));
    }
  }

  [[nodiscard]] LabelDistribution classify(Data const& d) const noexcept {
    LabelDistribution dist{};
    for (const auto& tree : forest_) {
      dist.combine(tree.classify(d));
    }
    return dist;
  }

 private:
  std::vector<Tree<Data>> forest_{};
};

}  // namespace rf
