#pragma once

namespace rf {

enum class SplitResult : uint8_t {
  LEFT = 0,
  RIGHT = 1,
};

template <typename Derived, typename Data>
class SplitCandidate {
 public:
  [[nodiscard]] SplitResult classify(Data const& d) const noexcept {
    return Derived::operator()(d);
  }
};

class SplitCandidateGenerator {};

}  // namespace rf
