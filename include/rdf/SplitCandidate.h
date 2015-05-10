#ifndef RGBD_RF_SPLITCANDIDATE_HH__
#define RGBD_RF_SPLITCANDIDATE_HH__

#include <rdf/common.h>
#include <rdf/Offset.h>

namespace rdf {

class SplitCandidate {
    public:
        Offset u;
        Offset v;
        float t;
        float g;

        SplitCandidate(
            const Offset uOff = Offset(0,0), 
            const Offset vOff = Offset(0,0), 
            const float thres = 0.0f,
            const float gain = 0.0f) 
            : u(uOff)
            , v(vOff)
            , t(thres)
            , g(gain) {}

        SplitCandidate (const SplitCandidate& sc) {
            u = sc.u;
            v = sc.v;
            t = sc.t;
            g = sc.g;
        }
};

} // namespace rdf

#endif // RGBD_RF_SPLITCANDIDATE_HH__
