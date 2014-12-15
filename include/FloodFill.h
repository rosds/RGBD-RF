# ifndef FLOOD_FILL
# define FLOOD_FILL

# include <RGBD-RF/common.hpp>

extern unsigned char color[7][3];

/**
 * Assing the new color to the current pixel and 
 * update the probabilty mask.
 * @param src Source image matrix.
 * @param forbidden Matrix of forbidden (already colored) pixels.
 * @param x Coordinate x of the pixel.
 * @param y Coordinate y of the pixel.
 * @param new_color New color for pixel (x,y).
 * @param labMask Probability mask per label.
 */
void assign_color(Mat& src, Mat& forbidden, 
                  int x, 
                  int y, 
                  Scalar new_color, 
                  vector <Mat>& labMask);

/**
 * Count the colors of the neighbors of pixel (x,y).
 * @param src Source image matrix.
 * @param x Coordinate x of the pixel.
 * @param y Coordinate y of the pixel.
 * @param col_c Color count array.
 */
void neighborhood_color(Mat& src, int x, int y, int *col_c);

/**
 * Fills the connected component with a given new color.
 * @param src source image matrix.
 * @param forbidden Matrix of forbidden (already colored) pixels.
 * @param x coordinate x of the pixel.
 * @param y coordinate y of the pixel.
 * @param original_color Current color of the connected component.
 * @param new_color New color of the connected component.
 * @param labMask Probability mask per label.
 * @return size of the colored connected component.
 */
int fill(Mat& src, 
         Mat& forbidden, 
         int x, 
         int y, 
         Scalar original_color, 
         Scalar new_color, 
         vector<Mat>& labMask);

/**
 * Forms a connected component from pixel (x,y).
 * @param src source image matrix.
 * @param v matrix of visited pixels.
 * @param x coordinate x of the pixel.
 * @param y coordinate y of the pixel.
 * @param col_c color count array.
 * @return size of the connected component.
 */
int flood_fill(Mat& src, Mat& v, int x, int y, int *col_c);


/** 
 * Procedure to mark pixels as forbiden.
 * @param src source image matrix.
 * @param f Matrix of forbidden pixels.
 * @param x coordinate x of the pixel.
 * @param y coordinate y of the pixel.
 * @param col_c color count array.
 */
void forbid(Mat& src, Mat& f, Mat& probMask, int x, int y, int col_c[]);

/**
 * Selects the color for the connected component based on the cantity
 * of neighbour colors.
 * @param col_c Color count array.
 * @param new_col New color of the connected component.
 */
void select_new_color (int *col_c, Scalar& new_col);

/**
 * BFS flood fill procedure.
 * @param src source image matrix.
 * @param labMask Probability mask per label.
 */
void flood_fill_filter(Mat& src, vector<Mat>& labMask, int threshold);

/**
 * Flood fill procedure.
 * @param src source image matrix.
 * @param labMask Probability mask per label.
 * @param iter Number of iterations of flood fill.
 */
void flood_fill_proc (Mat& src, 
                      vector <Mat>& labMask, 
                      int iter, 
                      int threshold);

/**
 * Flood fill procedure for testing results.
 * @param src source image matrix.
 * @param t_img train image to compare results.
 * @param labMask Probability mask per label.
 * @param iter Number of iterations of flood fill.
 */

float flood_fill_proc_test (Mat& src, 
                            Mat& t_img, 
                            vector <Mat>& labMask, 
                            int iter, 
                            int threshold);


# endif
