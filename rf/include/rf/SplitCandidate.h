#ifndef RF_SPLIT_CANDIDATE_H
#define RF_SPLIT_CANDIDATE_H

namespace rf {

template <typename Pixel>
class SplitCandidate {
   public:
    bool classify(Pixel const&) const noexcept {}
};

}  // namespace rf

#endif  // RF_SPLIT_CANDIDATE_H
