#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace rf {

class StringLabelMap;

/**
 *
 *  This is just some strong type for a classification label.
 *
 *  Labels are only ment to be use to separate or classify things. Inside the
 *  process of building the tree, the labels are only compared to each other,
 *  nothing more is expected from them.
 *
 *  These things are not ment to be instantiated by the user directly, but
 *  instead us something like the StringLabelMap.
 *
 */
template <typename T = size_t>
class Label {
  friend class StringLabelMap;

  explicit Label(T const& value) : value_(value) {}
  explicit Label(T&& value) noexcept(
      std::is_nothrow_move_constructible<T>::value)
      : value_(std::move(value)) {}

  explicit operator T const &() const noexcept { return value_; }

 public:
  bool operator==(Label const& other) const noexcept {
    return value_ == other.value_;
  }

 private:
  T value_;
};

/**
 *
 *  Just a simple map for string <=> labels. Used to register labels and to map
 *  back and forward between strings and labels.
 *
 */
class StringLabelMap {
 public:
  using LabelType = Label<size_t>;

  StringLabelMap() = default;
  StringLabelMap(StringLabelMap const&) = delete;
  StringLabelMap& operator=(StringLabelMap const&) = delete;

  // Adds a label to the set
  void addLabel(std::string label);

  std::optional<LabelType> operator[](std::string_view string) const noexcept;

 private:
  std::unordered_map<std::string, LabelType> map_{};
};

}  // namespace rf
