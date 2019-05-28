#ifndef RF_LABEL_IMAGE_H
#define RF_LABEL_IMAGE_H

#include <rf/Image.h>

namespace rf {

struct DefaultLabelPolicy {
    using label_type = uint8_t;

    template <typename Value>
    static label_type decode(Value&& v) noexcept {
        return static_cast<label_type>(v);
    }
};

template <typename ImageT, typename LabelPolicy = DefaultLabelPolicy>
class LabelImage : public LabelPolicy {
   public:
    using label_type = typename LabelPolicy::label_type;

    LabelImage() = default;
    explicit LabelImage(ImageT&& img) : data_{std::move(img)} {}
    explicit LabelImage(ImageT const& img) : data_{img} {}

    std::optional<label_type> operator()(int row, int col) const noexcept {
        const auto pixel = data_(row, col);
        if (pixel) {
            return LabelPolicy::decode(pixel.value());
        } else {
            return std::nullopt;
        }
    }

   private:
    ImageT data_{};
};

}  // namespace rf

#endif  // RF_LABEL_IMAGE_H
