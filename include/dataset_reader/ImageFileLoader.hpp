#ifndef IMAGE_FILE_LOADER_HH
#define IMAGE_FILE_LOADER_HH

#include <rf/Image.h>

#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fs = boost::filesystem;

struct ImageFileLoader {
    template <typename PixelT>
    static rf::Image<PixelT> Load(fs::path const& path) {
        auto img = cv::imread(path.string(),
                              CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

        if constexpr (std::is_same_v<PixelT, float>) {
            img.convertTo(img, CV_32F);
        }

        return {static_cast<size_t>(img.cols), static_cast<size_t>(img.rows),
                img.begin<PixelT>(), img.end<PixelT>()};
    }
};

#endif  // IMAGE_FILE_LOADER_HH
