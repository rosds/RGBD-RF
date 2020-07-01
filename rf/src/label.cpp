#include "rf/core/label.h"

namespace rf {

void StringLabelMap::addLabel(std::string label) {
  map_.emplace(std::move(label), LabelType(map_.size()));
}

std::optional<StringLabelMap::LabelType> StringLabelMap::operator[](
    std::string_view string) const noexcept {
  return std::nullopt;
}

}  // namespace rf
