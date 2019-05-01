#ifndef RF_IMAGE_FILE_LOADER_HH
#define RF_IMAGE_FILE_LOADER_HH

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace rf {

class ImageFileLoader {
    std::string path_;

   public:
    ImageFileLoader() = default;

    void path(std::string_view p) noexcept { path_ = p; }
    std::string_view path() const noexcept { return path_.c_str(); }

    template <class Image>
    void load(Image* img) const {
        *img = cv::imread(path_.c_str(),
                          CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
    }
};

}  // namespace rf

#endif /* end of include guard: RF_IMAGE_FILE_LOADER_HH */
