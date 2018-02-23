#ifndef RF_IMAGE_HH
#define RF_IMAGE_HH

#include <cassert>
#include <vector>
#include <mutex>
#include <rf/ImageFileLoader.hpp>

namespace rf {

template <class T, class Loader>
class Image : public Loader {
  size_t _w;
  size_t _h;
  std::vector<T> _v{};
  std::once_flag _load_image_flag;

  void load_image() noexcept {
    Loader::load(_w, _h, std::back_inserter(_v));  
  }

 public:
  T operator()(size_t x, size_t y) noexcept {
    std::call_once(_load_image_flag, &Image::load_image, this);
    assert(y * _w + x < _v.size());
    return _v[y * _w + x];
  }
};

using DepthImage = Image<float, ImageFileLoader<float>>;
using LabelImage = Image<uint8_t, ImageFileLoader<uint8_t>>;

} /* rf  */

#endif /* end of include guard: RF_IMAGE_HH */
