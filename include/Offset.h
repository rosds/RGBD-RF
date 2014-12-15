/**
 *  @file Offset.h
 *
 *  @brief This file contain the definition about the offset structure.
 *  This is the structure used to represent the offsets in pixels
 *  applied in the Feature calculation @see RandomForest.h
 *
 */

# ifndef OFFSET_H
# define OFFSET_H

# include <RGBD-RF/common.hpp>

# define MILLIMETERS_CONVERT 1000

class Offset {
    public:
        int x;
        int y;
        Offset(const int xOff = 0, const int yOff = 0) 
            : x(xOff), y(yOff) {}

        /**
         *  This constructor generate a Offset randomly between the
         *  range of numbers given.
         *
         *  @param range of numbers in where the offset will be
         *  generated randomly.
         */
        Offset(NumRange r);

        Offset& operator /= (const float& f);
        Offset& operator /= (const unsigned& u);
};

# endif
