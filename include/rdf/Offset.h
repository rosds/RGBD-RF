/**
 *  \file Offset.h
 *
 *  \brief This file contain the definition about the offset structure.
 *  This is the structure used to represent the offsets in pixels
 *  applied in the Feature calculation @see RandomForest.h
 *
 */

#ifndef RGBD_RF_OFFSET_HH__
#define RGBD_RF_OFFSET_HH__

#include <algorithm>

#define MILLIMETERS_CONVERT 1000

namespace rdf {

/** \brief Represents an offset amount in 2D
 */
class Offset {
    public:

        /** \brief Constructor.
         *  \param[in] xOff Offset in the x direction.
         *  \param[in] yOff Offset in the y direction.
         */
        Offset(const int xOff = 0, const int yOff = 0) 
            : x(xOff), y(yOff) {}

        /** \brief Destructor. **/
        virtual ~Offset() {}

        /** \brief set the offsets randomly within the specified range.
         *  \param[in] min Minimum integer of the range.
         *  \param[in] max Maximum integer of the range.
         */
        void setRandomlyInRange(const int min, const int max);

        /** \brief Divides both offsets by the given float.
         *  \param[in] f The float to divide both offsets.
         *  \return The offset with both coordinates divided by the float.
         */
        Offset& operator/=(const float f);
        Offset operator/(const float f) const;

        /** \brief Divides both offsets by the given float.
         *  \param[in] f The float to divide both offsets.
         *  \return The offset with both coordinates divided by the float.
         */
        Offset& operator/=(const unsigned& u);
        Offset operator/(const unsigned& u) const;

        int x;
        int y;
};

} // namespace rdf

# endif // RGBD_RF_OFFSET_HH__
