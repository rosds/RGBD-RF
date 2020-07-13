#pragma once

#include <cstdlib>

namespace rf {

struct TreeParameters {
  size_t numberOfTrees;
  size_t minSamplesPerNode;
  size_t maxDepth;
  size_t candidatesToGeneratePerNode;
};

}  // namespace rf
