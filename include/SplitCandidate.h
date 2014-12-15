
# ifndef SPLITCANDIDATE_H
# define SPLITCANDIDATE_H

# include <RGBD-RF/common.hpp>
# include "Offset.h"

class SplitCandidate {
    public:
        Offset u;
        Offset v;
        float t;
        float g;
        SplitCandidate (const Offset uOff = Offset(0,0), 
                        const Offset vOff = Offset(0,0), 
                        const float thres = 0.0,
                        const float gain = 0.0) 
            : u(uOff), v(vOff), t(thres), g(gain) {}

        SplitCandidate (const SplitCandidate& sc) {
            u = sc.u;
            v = sc.v;
            t = sc.t;
            g = sc.g;
        }
};

# endif
