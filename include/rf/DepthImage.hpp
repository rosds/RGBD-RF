#ifndef RF_DEPTH_IMAGE_HH
#define RF_DEPTH_IMAGE_HH

#include <iostream>
#include <algorithm>

namespace rf {

template <size_t W, size_t H>
struct DepthImage {
  const size_t width = W;
  const size_t height = H;
  std::array<float, W * H> values{};

  constexpr float operator()(size_t x, size_t y) const {
    return values[y * W + x];
  }

  template <class InputIter>
  constexpr DepthImage(InputIter first, InputIter last) {
    std::copy(first, last, values.begin()); 
  }

  friend std::ostream& operator<<(std::ostream& out, const DepthImage& di) {
    out << '[';
    for (const float value : di.values) {
      out << value << ','; 
    } 
    return out << ']';
  }
};

} /* rf  */

#endif /* end of include guard: RF_DEPTH_IMAGE_HH */
