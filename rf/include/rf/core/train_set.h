#pragma once

namespace rf {

/**
 *
 *   This is a interface that has to be defined by the user. It intends to work
 *   as a sort of random access iterator. It will be used to fetch training
 *   examples from whatever pool of data is behind of this.
 *
 */
template <typename InputData, typename LabelType>
class TrainSet {
 public:
  using TrainingExample = std::pair<InputData, LabelType>;
  virtual ~TrainSet() {}
  virtual TrainingExample sample() = 0;
};

}  // namespace rf
