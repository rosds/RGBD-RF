#ifndef RF_IMAGE_HH
#define RF_IMAGE_HH

#include <cassert>
#include <cstdint>
#include <rf/ImageFileLoader.hpp>
#include <rf/Pixel.hpp>
#include <vector>

#include <opencv2/core/core.hpp>

namespace rf {

template <typename _Tp, class Loader>
class Image final : public cv::Mat_<_Tp>, public Loader {
   public:
    using value_type = _Tp;

    /** \brief Load the image content into this structure
     *
     */
    void load() { Loader::load(static_cast<cv::Mat_<_Tp>*>(this)); }

    Pixel<Image> pixel(uint32_t x, uint32_t y) const noexcept {
        return Pixel<Image>(x, y, *this);
    }

    operator cv::Mat() const { return *static_cast<cv::Mat_<_Tp>*>(this); }
};

using DepthImage = Image<float, ImageFileLoader>;
using LabelImage = Image<uint8_t, ImageFileLoader>;

}  // namespace rf

#endif /* end of include guard: RF_IMAGE_HH */
