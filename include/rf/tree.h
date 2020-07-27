#pragma once

#include <rf/label.h>
#include <rf/label_distribution.h>
#include <rf/parameters.h>
#include <rf/split_candidate.h>
#include <rf/train_set.h>

#include <algorithm>
#include <memory>
#include <unordered_map>

namespace rf {

template <typename Data>
class TreeNode {
 public:
  virtual LabelDistribution classify(Data const&) const noexcept = 0;
};

template <typename Data>
using NodePtr = std::unique_ptr<TreeNode<Data>>;

template <typename Data, typename SplitCandidate>
class SplitNode : public TreeNode<Data> {
 public:
  using Ptr = NodePtr<Data>;

  LabelDistribution classify(Data const& data) const noexcept override {
    if (split_.classify(data) == SplitResult::LEFT) {
      return left_->classify(data);
    } else {
      return right_->classify(data);
    }
  }

  SplitNode(SplitCandidate&& split) noexcept(
      std::is_nothrow_constructible_v<SplitCandidate>)
      : split_(std::move(split)) {}

  void setLeftChild(Ptr&& left) { left_ = std::move(left); }
  void setRightChild(Ptr&& right) { right_ = std::move(right); }

 private:
  SplitCandidate split_;
  Ptr left_{nullptr};
  Ptr right_{nullptr};
};

template <typename Data>
class LeafNode : public TreeNode<Data> {
 public:
  LabelDistribution classify(Data const& data) const noexcept override {
    return distribution_;
  }

  template <typename InputIterator>
  LeafNode(InputIterator begin, InputIterator end)
      : distribution_{begin, end} {}

 private:
  LabelDistribution distribution_{};
};

template <typename Data>
class Tree {
 public:
  LabelDistribution classify(Data const& data) const noexcept {
    return root_->classify(data);
  }

  Tree(NodePtr<Data>&& root) noexcept : root_{std::move(root)} {}

 private:
  NodePtr<Data> root_{nullptr};
};

template <typename SplitCandidate, typename InputIterator>
double evaluateSplitCandidate(SplitCandidate const& candidate,
                              InputIterator begin, InputIterator end) {
  const auto entireSet = LabelDistribution{begin, end};
  const auto totalEntropy = entireSet.entropy();

  auto split = std::partition(begin, end, [&candidate](const auto& example) {
    return candidate.classify(example.first) == SplitResult::LEFT;
  });

  auto leftSet = LabelDistribution{begin, split};
  auto rightSet = LabelDistribution{split, end};
  auto leftTotal = static_cast<double>(std::distance(begin, split));
  auto rightTotal = static_cast<double>(std::distance(split, end));
  auto entireTotal = static_cast<double>(std::distance(begin, end));

  // Information gain
  const double informationGain =
      totalEntropy - ((leftTotal / entireTotal) * leftSet.entropy() +
                      (rightTotal / entireTotal) * rightSet.entropy());

  return informationGain;
}

template <typename SplitCandidate, typename InputIterator>
SplitCandidate getBestCandidate(InputIterator begin, InputIterator end,
                                size_t n) {
  double maxScore = 0.0;
  SplitCandidate bestCandidate{};
  for (size_t i = 0; i < n; ++i) {
    auto candidate = SplitCandidate::generate();
    auto score = evaluateSplitCandidate(candidate, begin, end);
    if (score > maxScore) {
      bestCandidate = candidate;
      maxScore = score;
    }
  }

  return bestCandidate;
}

template <typename SplitCandidate, typename InputIterator,
          typename TrainingExample = typename InputIterator::value_type,
          typename Data = typename TrainingExample::first_type>
NodePtr<Data> trainNode(InputIterator begin, InputIterator end,
                        TreeParameters conf,
                        size_t currentDepth) {  // stop criteria
  if (currentDepth > conf.maxDepth ||
      std::distance(begin, end) < conf.minSamplesPerNode) {
    return std::make_unique<LeafNode<Data>>(begin, end);
  }

  // Generate a split node
  auto candidate = getBestCandidate<SplitCandidate>(
      begin, end, conf.candidatesToGeneratePerNode);

  // reorder the samples according to the split candidate
  auto mid = std::partition(begin, end, [&candidate](auto const& sample) {
    return candidate.classify(sample.first) == SplitResult::LEFT;
  });

  if (mid == begin || mid == end) {
    return std::make_unique<LeafNode<Data>>(begin, end);
  }

  // Create the splitnode and continue training the children
  auto splitNode =
      std::make_unique<SplitNode<Data, SplitCandidate>>(std::move(candidate));
  splitNode->setLeftChild(
      trainNode<SplitCandidate>(begin, mid, conf, currentDepth + 1));
  splitNode->setRightChild(
      trainNode<SplitCandidate>(mid, end, conf, currentDepth + 1));

  return splitNode;
}

template <typename SplitCandidate, typename InputData>
Tree<InputData> trainTree(TrainSet<InputData>& train,
                          TrainSet<InputData>& validation,
                          TreeParameters stoppingCriteria) {
  auto samples = train.sample();
  // stop building tree
  return Tree<InputData>(trainNode<SplitCandidate>(
      samples.begin(), samples.end(), stoppingCriteria, 0));
}

template <typename Classifier, typename Data>
double evaluateTree(Classifier const& tree, rf::TrainSet<Data>& test) {
  double error = 0.0;
  double count = 0.0;
  auto iter = test.iter();

  auto maxProb = [](auto const& p, auto const& g) {
    return p.second < g.second;
  };

  while (true) {
    auto value = iter->value();
    if (!value.has_value()) {
      break;
    }

    auto const& trainExample = value.value();
    auto const dist = tree.classify(trainExample.first);

    const auto maxElement = std::max_element(dist.begin(), dist.end(), maxProb);
    if (maxElement->first != trainExample.second) {
      error += 1.0;
    }

    count += 1.0;

    iter->next();
  }

  return error / count;
}

}  // namespace rf

#include "impl/tree.hpp"
