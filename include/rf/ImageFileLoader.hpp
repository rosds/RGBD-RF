#ifndef RF_IMAGE_FILE_LOADER_HH
#define RF_IMAGE_FILE_LOADER_HH

#include <algorithm>
#include <cstdint>
#include <experimental/filesystem>
#include <string_view>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fs = std::experimental::filesystem;

namespace rf {

class ImageFileLoader {
  fs::path _path;

 public:
  ImageFileLoader() = default;

  void path(std::string_view p) noexcept { _path = p; }
  std::string_view path() const noexcept { return _path.c_str(); }

  template <class Image>
  void load(Image* img) const {
    *img = cv::imread(_path.c_str(),
                     CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  }
};

} /* rf  */

#endif /* end of include guard: RF_IMAGE_FILE_LOADER_HH */
