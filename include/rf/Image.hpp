#ifndef RF_IMAGE_HH
#define RF_IMAGE_HH

#include <cstdint>
#include <cassert>
#include <vector>
#include <mutex>
#include <rf/ImageFileLoader.hpp>
#include <rf/Pixel.hpp>

namespace rf {

template <class T, class Loader>
class Image : public Loader {
  mutable uint32_t _w;
  mutable uint32_t _h;
  mutable std::vector<T> _v{};
  mutable std::once_flag _load_image_flag;

  void load_image() const noexcept {
    Loader::load(_w, _h, std::back_inserter(_v));  
  }

 public:
  using value_type = T;

  T const& operator()(uint32_t x, uint32_t y) const noexcept {
    std::call_once(_load_image_flag, &Image::load_image, this);
    assert(y * _w + x < _v.size());
    return _v[y * _w + x];
  }

  Pixel<Image> pixel(uint32_t x, uint32_t y) const noexcept {
    assert(y * _w + x < _v.size());
    return Pixel<Image>(x, y, *this); 
  }
};

using DepthImage = Image<float, ImageFileLoader<float>>;
using LabelImage = Image<uint8_t, ImageFileLoader<uint8_t>>;

} /* rf  */

#endif /* end of include guard: RF_IMAGE_HH */
