# include "Offset.h"

Offset& Offset :: operator /= (const float& f)
{
    if (f != 0.0f) {
        this -> x /= f;
        this -> y /= f;
    }
    return *this;
}

Offset& Offset :: operator /= (const unsigned& u)
{
    if (u != 0) {
        this -> x /= u;
        this -> y /= u;
    }
    return *this;
}

/**
 *  This constructor generate a Offset randomly between the
 *  range of numbers given.
 *
 *  @param range of numbers in where the offset will be
 *  generated randomly.
 */
 //CHECK: anque no me cuadra tanto mult por 1000 asi caiman.

Offset :: Offset(NumRange r)
{
    r.start *= MILLIMETERS_CONVERT;
    r.end   *= MILLIMETERS_CONVERT;

    r.start -= MILLIMETERS_CONVERT;
    r.end   += MILLIMETERS_CONVERT;

    r.end++;
    
    x = int(randFloat(r)) + MILLIMETERS_CONVERT;
    y = int(randFloat(r)) + MILLIMETERS_CONVERT;
}
