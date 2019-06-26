#ifndef RF_TRAIN_SET_H
#define RF_TRAIN_SET_H

#include <vector>

namespace rf {

template <typename TrainImage>
class TrainSet {
   public:
    TrainSet() = default;

    template <typename... Args>
    void emplace_back(Args&&... args) {
        images_.emplace_back(std::forward<Args>(args)...);
    }

   private:
    std::vector<TrainImage> images_;
};

}  // namespace rf

#endif  // RF_TRAIN_SET_H
