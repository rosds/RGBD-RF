#ifndef RF_IMAGE_HH
#define RF_IMAGE_HH

#include <cstdint>
#include <cassert>
#include <vector>
#include <rf/ImageFileLoader.hpp>
#include <rf/Pixel.hpp>

namespace rf {

template <class T, class Loader>
class Image : public Loader {
  uint32_t _w;
  uint32_t _h;
  std::vector<T> _v{};

 public:
  using value_type = T;

  void load() {
    Loader::load(_w, _h, std::back_inserter(_v));  
  }

  void release() noexcept {
    _v.clear(); 
  }

  T const& operator()(uint32_t x, uint32_t y) const noexcept {
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
