#ifndef RF_IMAGE_FILE_LOADER_HH
#define RF_IMAGE_FILE_LOADER_HH

#include <algorithm>
#include <vector>
#include <experimental/filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string_view>

namespace fs = std::experimental::filesystem;

namespace rf {

template <class T, class OutputIt = std::back_insert_iterator<std::vector<T>>>
class ImageFileLoader {
  fs::path _path;

 public:
  void path(std::string_view p) noexcept { _path = p; }
  std::string_view path() const noexcept { return _path.c_str(); }

  void load(size_t& w, size_t& h, OutputIt out) const noexcept {
    cv::Mat img = cv::imread(_path.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

    if constexpr (std::is_same<T, float>::value) {
      img.convertTo(img, CV_32F);
    }

    w = static_cast<size_t>(img.cols);
    h = static_cast<size_t>(img.rows);
    std::copy(img.begin<T>(), img.end<T>(), out);
  }
};

} /* rf  */

#endif /* end of include guard: RF_IMAGE_FILE_LOADER_HH */
