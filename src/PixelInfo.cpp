# include "PixelInfo.h"

PixelInfo :: PixelInfo (const Coord c, unsigned short pID)
{
    x  = c.first;
    y  = c.second;
    id = pID;
}

PixelInfo& PixelInfo :: operator += (const Offset& u)
{
    this -> x += u.x;
    this -> y += u.y;
    return *this;
}

const PixelInfo PixelInfo :: operator + (const Offset& u)
{
    PixelInfo result;
    result = *this;
    result += u;
    return result;
}

