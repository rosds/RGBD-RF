#pragma once
#include <rf/core/label.h>

#include <memory>
#include <optional>
#include <vector>

namespace rf {

template <typename Data>
using TrainingExample = std::pair<Data, Label>;

/**
 *
 *   This is a interface that has to be defined by the user. It intends to work
 *   as a sort of random access iterator. It will be used to fetch training
 *   examples from whatever pool of data is behind of this.
 *
 */
template <typename Data>
class TrainSet {
 public:
  using TrainingExampleType = TrainingExample<Data>;

  class TrainSetIterator {
   public:
    virtual void next() = 0;
    virtual std::optional<TrainingExampleType> value() = 0;
  };

  virtual std::vector<TrainingExampleType> sample() = 0;
  virtual std::unique_ptr<TrainSetIterator> iter() = 0;
};

}  // namespace rf
