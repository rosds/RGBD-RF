# include "PixelInfo.h"

rdf::PixelInfo& rdf::PixelInfo::operator+=(const Offset& u) {
    this->x += u.x;
    this->y += u.y;
    return *this;
}


const rdf::PixelInfo rdf::PixelInfo::operator+(const Offset& u) const {
    PixelInfo result(*this);
    result += u;
    return result;
}
