#ifndef RF_PIXEL_HH
#define RF_PIXEL_HH

#include <cinttypes>
#include <functional>
#include <optional>

namespace rf {

template <class Image>
class Pixel {
   public:
    using value_type = typename Image::value_type;

    /** \brief Return the image value for this pixel or not.
     *
     *  Return the value associated to this pixel if any. Otherwise, return
     *  std::nullopt.
     *
     *  \return The value of the pixel if any. std::nullopt otherwise.
     */
    value_type value() const { return value_.value(); }

    /** \brief No default constructor.
     *
     *  Pixel instances can only be obtained from images.
     */
    Pixel() = delete;

    bool has_value() const noexcept { return value_.has_value(); }

    operator bool() const { return has_value(); }

   private:
    Pixel(uint32_t x, uint32_t y, std::optional<value_type> value,
          std::reference_wrapper<const Image> img)
        : x_{x}, y_{y}, value_{value}, img_{img} {}

    uint32_t x_;
    uint32_t y_;
    std::optional<value_type> value_;
    std::reference_wrapper<const Image> img_;

    friend Pixel Image::operator()(int, int) const;
};

}  // namespace rf

#endif /* end of include guard: RF_PIXEL_HH */
