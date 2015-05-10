#include <rdf/common.h>

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
float randFloat(NumRange r)
{
    double scaled = (double) rand() / (double)((unsigned)RAND_MAX + 1);
    return r.start + r.size() * scaled;
}

/**
 *  Take initial time.
 *  @return initial time
 */
double takeInitialTime()
{
    struct timeval t_p;
    double t_initial;

    //Getting Time

    if(!gettimeofday(&t_p,NULL))
        t_initial = (double)t_p.tv_sec + ((double)t_p.tv_usec)/1000000.0;
    else
        printf("\nMal Tiempo...!\n");

    return t_initial;
}

/**
 *  Take final time.
 *  @return final time.
 */
void takeFinalTime(double t_initial)
{
    struct timeval t_p;
    double t_final;
    if(!gettimeofday(&t_p,NULL))
        t_final = (double)t_p.tv_sec + ((double)t_p.tv_usec)/1000000.0;
    else
        printf("\nMal Tiempo...!\n");

    printf("Tiempo de corrida: %1.8f\n", t_final-t_initial);

}


/** \brief Return an unsorted vector of indices
 *
 *  \param[in] size Size of the resulting vector of indices.
 *  \return An unordered vector of indices of the specified size.
 */
std::vector<int> permutation(const int size) {
    std::vector<int> indices(size);

    // Set the index vector from 0 to size.
    std::iota(std::begin(indices), std::end(indices), 0);

    // Shuffle the indices randomly
    std::random_shuffle(std::begin(indices), std::end(indices));

    return indices;
}


/**
 * Convert to real world coordinates.
 */
Vec3f DepthToWorld(int x, int y, unsigned int  depthValue)
{
    static const double fx_d = 1.0 / FX_D;
    static const double fy_d = 1.0 / FY_D;
    static const double cx_d = CX_D;
    static const double cy_d = CY_D;

    Vec3f result;
    const double depth = double(depthValue) / 1000.0; // to meters
    result[0] = float(float(x - cx_d) * depth * fx_d);
    result[1] = float(float(y - cy_d) * depth * fy_d);
    result[2] = float(depth);
    return result;
}
