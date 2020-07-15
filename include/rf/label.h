#pragma once

#include <stdint.h>

#include <algorithm>
#include <vector>

namespace rf {
using Label = uint8_t;

/**
 *  This is just a convenient class to map external labels to internal labels
 */
template <typename T>
class LabelRegistry {
  LabelRegistry() = default;

 public:
  static LabelRegistry& instance() {
    static LabelRegistry<T> instance{};
    return instance;
  }

  [[nodiscard]] Label getLabel(T const& t) noexcept {
    auto it = std::find_if(map_.begin(), map_.end(),
                           [&t](auto const& p) { return p.first == t; });
    if (it != map_.end()) {
      return it->second;
    } else {
      map_.emplace_back(t, map_.size());
      return map_.back().second;
    }
  }

 private:
  std::vector<std::pair<T, Label>> map_{};
};
}  // namespace rf
