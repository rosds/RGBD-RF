#ifndef RF_DEPTH_IMAGE_HH
#define RF_DEPTH_IMAGE_HH

#include <algorithm>
#include <cassert>
#include <vector>

namespace rf {

class DepthImage final {
  size_t _w;
  size_t _h;
  std::vector<float> _v{};

 public:
  float operator()(size_t x, size_t y) const noexcept {
    assert(y * _w + x < _v.size());
    return _v[y * _w + x];
  }

  template <class InputIter>
  DepthImage(size_t width, size_t height, InputIter first, InputIter last)
      : _w{width}, _h{height} {
    assert(_w * _h == last - first);
    std::copy(first, last, std::back_inserter(_v));
  }
};

} /* rf  */

#endif /* end of include guard: RF_DEPTH_IMAGE_HH */
