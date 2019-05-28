#ifndef RF_TRAINING_IMAGE_H
#define RF_TRAINING_IMAGE_H

#include <rf/Image.h>

namespace rf {

template <typename SourceImage, typename LabelImage>
class TrainingImage {
   public:
    TrainingImage(SourceImage&& src, LabelImage&& labels)
        : source_{std::forward<SourceImage>(src)},
          labels_{std::forward<LabelImage>(labels)} {}

   private:
    SourceImage source_;
    LabelImage labels_;
};

}  // namespace rf

#endif  // RF_TRAINING_IMAGE_H
