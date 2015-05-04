# include <Offset.h>

/** \brief Divides both offsets by the given float.
 *  \param[in] f The float to divide both offsets.
 *  \return The offset with both coordinates divided by the float.
 */
rdf::Offset& rdf::Offset::operator/=(const float f) {
    x = static_cast<int>(floor(static_cast<float>(x) / std::max(f, std::numeric_limits<float>::epsilon())));
    y = static_cast<int>(floor(static_cast<float>(y) / std::max(f, std::numeric_limits<float>::epsilon())));
    return *this;
}

rdf::Offset rdf::Offset::operator/(const float f) const {
    int a = static_cast<int>(floor(static_cast<float>(x) / std::max(f, std::numeric_limits<float>::epsilon())));
    int b = static_cast<int>(floor(static_cast<float>(y) / std::max(f, std::numeric_limits<float>::epsilon())));
    return Offset(a, b);
}

rdf::Offset& rdf::Offset::operator/=(const unsigned& u) {
    if (u != 0) {
         x /= u;
         y /= u;
    }
    return *this;
}

rdf::Offset rdf::Offset::operator/(const unsigned& u) const {
    return (u != 0)? Offset(x / u, y / u) : Offset();
}

/** \brief set the offsets randomly within the specified range.
 *  \param[in] min Minimum integer of the range.
 *  \param[in] max Maximum integer of the range.
 */
void rdf::Offset::setRandomlyInRange(const int min, const int max) {
    x = min + (rand() % (max - min + 1));
    y = min + (rand() % (max - min + 1));
}
