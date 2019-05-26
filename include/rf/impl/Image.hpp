namespace rf {

template <typename PixelT>
std::optional<typename Image<PixelT>::pixel_type> Image<PixelT>::operator()(
    int row, int col) const noexcept {
    if ((0 <= row) && (row < rows_) && (0 <= col) && (col < cols_)) {
        return {pixel_type(row, col, std::cref(*this))};
    } else {
        return std::nullopt;
    }
}

}  // namespace rf
