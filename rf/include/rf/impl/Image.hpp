#include <rf/Image.h>

namespace rf {

template <typename PixelT>
typename Image<PixelT>::pixel_type Image<PixelT>::operator()(int row,
                                                             int col) const
    noexcept {
    return {pixel_type(row, col, value(row, col), std::cref(*this))};
}

template <typename PixelT>
std::optional<typename Image<PixelT>::value_type> Image<PixelT>::value(
    int row, int col) const noexcept {
    if ((0 <= row) && (row < rows_) && (0 <= col) && (col < cols_)) {
        return {data_[row * cols_ + col]};
    } else {
        return std::nullopt;
    }
}

}  // namespace rf
