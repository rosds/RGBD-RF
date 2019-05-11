#ifndef RF_IMAGE_HH
#define RF_IMAGE_HH

#include <rf/Pixel.hpp>

#include <optional>
#include <vector>

namespace rf {

template <typename PixelT>
class Image {
   public:
    using value_type = PixelT;

    Image() = default;

    template <typename... Args>
    Image(size_t rows, size_t cols, Args&&... args)
        : rows_{rows}, cols_{cols}, data_{std::forward<Args>(args)...} {}

    std::optional<PixelT> operator()(int row, int col) const noexcept {
        if ((0 <= row) && (row < rows_) && (0 <= col) && (col < cols_)) {
            return {data_[row * cols_ + col]};
        } else {
            return std::nullopt;
        }
    }

    size_t cols() const noexcept { return cols_; }
    size_t rows() const noexcept { return rows_; }
    operator std::vector<PixelT>() { return data_; }

   private:
    size_t rows_{0};
    size_t cols_{0};
    std::vector<PixelT> data_{};
};

using DepthImage = Image<float>;
using LabelImage = Image<uint8_t>;

}  // namespace rf

#endif /* end of include guard: RF_IMAGE_HH */
