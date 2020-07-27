#include <rf/tree.h>

namespace rf {
namespace impl {

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

template <typename SplitCandidate, typename InputIterator>
SplitCandidate getBestCandidate(InputIterator begin, InputIterator end,
                                size_t n);

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

}  // namespace impl

template <typename SplitCandidate, typename InputData>
Tree<InputData> trainTree(TrainSet<InputData>& train,
                          TrainSet<InputData>& validation,
                          TreeParameters stoppingCriteria) {
  auto samples = train.sample();
  // stop building tree
  return Tree<InputData>(impl::trainNode<SplitCandidate>(
      samples.begin(), samples.end(), stoppingCriteria, 0));
}
}  // namespace rf
