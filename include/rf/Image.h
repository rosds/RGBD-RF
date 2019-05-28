#ifndef RF_IMAGE_HH
#define RF_IMAGE_HH

#include <rf/Pixel.h>

#include <optional>
#include <vector>

namespace rf {

template <typename PixelT>
class Image {
   public:
    using value_type = PixelT;
    using pixel_type = Pixel<Image<PixelT>>;

    Image() = default;

    template <typename... Args>
    Image(size_t rows, size_t cols, Args&&... args)
        : rows_{rows}, cols_{cols}, data_{std::forward<Args>(args)...} {}

    std::optional<pixel_type> operator()(int row, int col) const noexcept;

    size_t cols() const noexcept { return cols_; }
    size_t rows() const noexcept { return rows_; }

    friend pixel_type;

   protected:
    value_type const& value(int row, int col) const noexcept {
        return data_[row * cols_ + col];
    }

   private:
    size_t rows_{0};
    size_t cols_{0};
    std::vector<PixelT> data_{};
};

using DepthImage = Image<float>;

}  // namespace rf

#include "impl/Image.hpp"

#endif /* end of include guard: RF_IMAGE_HH */
