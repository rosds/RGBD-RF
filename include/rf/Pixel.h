#ifndef RF_PIXEL_HH
#define RF_PIXEL_HH

#include <cinttypes>
#include <functional>
#include <optional>

namespace rf {

struct Offset {
    uint32_t x;
    uint32_t y;
};

template <class Image>
class Pixel {
   public:
    using value_type = typename Image::value_type;

    value_type const& value() const noexcept {
        return img_.get().value(x_, y_);
    }

    Pixel() = delete;
    friend std::optional<Pixel> Image::operator()(int, int) const;

   private:
    Pixel(uint32_t x, uint32_t y, std::reference_wrapper<const Image> img)
        : x_{x}, y_{y}, img_{img} {}

    uint32_t x_;
    uint32_t y_;
    std::reference_wrapper<const Image> img_;
};

}  // namespace rf

#endif /* end of include guard: RF_PIXEL_HH */
