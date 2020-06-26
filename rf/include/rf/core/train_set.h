#pragma once

namespace rf {

template <typename Data, typename Label>
struct TrainExample {};

template <typename TrainExample>
class TrainSet {
 public:
  TrainExample getRandomTrainExample() {}
};

}  // namespace rf
