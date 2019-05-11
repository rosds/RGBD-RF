#ifndef RF_IMAGE_FILE_LOADER_HH
#define RF_IMAGE_FILE_LOADER_HH

#include <rf/Image.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string_view>

namespace rf {

struct ImageFileLoader {
    template <typename PixelT>
    static Image<PixelT> Load(std::string_view const& path) {
        auto img = cv::imread(path.data(),
                              CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

        auto copy = std::vector<PixelT>(img.begin<PixelT>(), img.end<PixelT>());

        return {static_cast<size_t>(img.cols), static_cast<size_t>(img.rows),
                std::move(copy)};
    }
};

}  // namespace rf

#endif /* end of include guard: RF_IMAGE_FILE_LOADER_HH */
