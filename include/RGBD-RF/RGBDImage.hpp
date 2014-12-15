#ifndef RGBD_RF_RGBDIMAGE_HH
#define RGBD_RF_RGBDIMAGE_HH

#include <vector>

namespace rf {

class RGBDImage {
public:
    RGBDImage();
    ~RGBDImage();

private:
    unsigned width, height;
    std::vector<float> depth;
};

} // namespace rf

#endif // RGBD-RF_RGBDIMAGE_HH
