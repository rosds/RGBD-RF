#pragma once

#include <type_traits>

namespace rf {

/**
 *
 *  Labels are only ment to be use to separate or classify things. Inside the
 *  process of building the tree, the labels are only compared to each other,
 *  nothing more is expected from them.
 *
 */
template <typename Tag, typename T = size_t>
class Label {
 public:
  using underlying_type = T;

  explicit Label(T const& value) : value_(value) {}
  explicit Label(T&& value) noexcept(
      std::is_nothrow_move_constructible<T>::value)
      : value_(std::move(value)) {}

  bool operator==(Label const& other) const noexcept {
    return value_ == other.value_;
  }

  explicit operator T const &() const noexcept { return value_; }

 private:
  T value_;
};

}  // namespace rf
