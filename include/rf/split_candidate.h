#pragma once

namespace rf {

enum class SplitResult : uint8_t {
  LEFT = 0,
  RIGHT = 1,
};

template <typename Derived, typename Data>
class SplitCandidate {
 public:
  using Input = Data;

  [[nodiscard]] SplitResult classify(Data const& d) const noexcept {
    return static_cast<Derived const&>(*this)(d);
  }

  static SplitCandidate generate() { return Derived::generate(); }
};

}  // namespace rf
