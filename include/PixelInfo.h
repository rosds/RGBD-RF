
# ifndef PIXELINFO_H
# define PIXELINFO_H

# include "common.h"
# include "Offset.h"

class PixelInfo {
    public:
        unsigned short id;
        unsigned short x;
        unsigned short y;

        PixelInfo (const unsigned short xCoord = 0u, 
                    const unsigned short yCoord = 0u)
            : x(xCoord), y(yCoord) {}

        PixelInfo (const Coord c, unsigned short pID = 0);

        PixelInfo& operator += (const Offset& u);

        const PixelInfo operator + (const Offset& u);
};

# endif
