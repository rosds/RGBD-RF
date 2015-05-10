/** \file common.h
 *
 *  \brief This file contain the include headers for the use of the
 *  program and other common definitions.
 */

#ifndef RGBD_RF_COMMON_HH__
#define RGBD_RF_COMMON_HH__

// ----------------------------------------------------------------------
// C++ include headers
// ----------------------------------------------------------------------

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <utility>
#include <ctime>
#include <sys/time.h>
#include <math.h>
#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include <algorithm>
#include <dirent.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <queue>

#include <random>
#include <numeric>

using namespace cv;

// ----------------------------------------------------------------------
// Some typedefs
// ----------------------------------------------------------------------

typedef std::pair<unsigned short, unsigned short> Coord;
typedef uint8_t Label;

// ----------------------------------------------------------------------
// Some Globals
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Basic common structures
// ----------------------------------------------------------------------

/**
 *  This structure is used to specify a range of numbers. The range
 *  start at the integer number 'start' and finish in the integer number
 *  'end'.
 */
class NumRange {
    public:
        int start;
        int end;

        // Constructor
        NumRange(int s = 0, int e = 0) : start(s), end(e) {}

        int size () { return end - start; }
};

/**
 *  randFloat
 *
 *  This function simply generate a random float given a range of
 *  integer values.
 *  
 *  @param r is the range of integer values.
 *
 *  @return the random float generated.
 */
float randFloat(NumRange r);

/**
 *  Take initial time.
 *  @return initial time
 */
double takeInitialTime();

/**
 *  Take final time.
 *  @return final time.
 */
void takeFinalTime(double t_initial);

/**
 * Convert to real world coordinates.
 */
Vec3f DepthToWorld(int x, int y, unsigned int depthValue);


/** \brief Return an unsorted vector of indices
 *
 *  \param[in] size Size of the resulting vector of indices.
 *  \return An unordered vector of indices of the specified size.
 */
std::vector<int> permutation(const int size);

// ----------------------------------------------------------------------
// Random Forest Macros
// ----------------------------------------------------------------------

# define WIDTH 640
# define HEIGHT 480
# define THREADS_PER_NODE 2

// ----------------------------------------------------------------------
// Image configuration macros
// ----------------------------------------------------------------------

# define NUMBER_OF_LABELS 6

// ----------------------------------------------------------------------
// Kinect Parameters
// ----------------------------------------------------------------------

//COLOR

#define FX_RGB 5.2921508098293293e+02
#define FY_RGB 5.2556393630057437e+02
#define CX_RGB 3.2894272028759258e+02
#define CY_RGB 2.6748068171871557e+02
#define K1_RGB 2.6451622333009589e-01
#define K2_RGB -8.3990749424620825e-01
#define P1_RGB -1.9922302173693159e-03
#define P2_RGB 1.4371995932897616e-03
#define K3_RGB 9.1192465078713847e-01

#define FX_D 5.9421434211923247E+02
#define FY_D 5.9104053696870778E+02
#define CX_D 3.3930780975300314E+02
#define CY_D 2.4273913761751615E+02
#define K1_D -2.6386489753128833E-01
#define K2_D 9.9966832163729757E-01
#define P1_D -7.6275862143610667E-04
#define P2_D 5.0350940090814270E-03
#define K3_D -1.3053628089976321E+00


#endif // RGBD_RF_COMMON_HH__
