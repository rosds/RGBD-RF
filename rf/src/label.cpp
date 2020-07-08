#include "rf/core/label.h"

namespace rf {

StringLabelMap::LabelType StringLabelMap::toLabel(std::string label) {
  auto entry = map_.emplace(std::move(label), LabelType(map_.size()));
  return entry.first->second;
}

}  // namespace rf
