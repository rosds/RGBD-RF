#ifndef RGBD_RF_PIXELINFO_HH__
#define RGBD_RF_PIXELINFO_HH__

#include <stdint.h>
#include <Offset.h>

namespace rdf {

/** \brief Represents one pixel in the image
 */
class PixelInfo {
    public:
        PixelInfo(
            const uint32_t row = 0u, 
            const uint32_t col = 0u,
            const uint32_t img_id = 0u)
            : x(row)
            , y(col)
            , id(img_id) {}

        PixelInfo& operator+=(const Offset& u);

        const PixelInfo operator+(const Offset& u) const;

        uint32_t id;
        uint32_t x;
        uint32_t y;
};

}

# endif // RGBD_RF_PIXELINFO_HH__
