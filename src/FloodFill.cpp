#include "FloodFill.h"

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
void assign_color(Mat& src, 
                  Mat& forbidden, 
                  int x, 
                  int y, 
                  Scalar new_color, 
                  vector <Mat>& labMask) 
{

    Scalar ncolor = Scalar(src.at<Vec3b>(x,y));

    for (int i = 1; i <= NUMBER_OF_LABELS; i++) {

        if (ncolor == Scalar(color[i][2],color[i][1],color[i][0])) {
            labMask[i - 1].at<uchar>(x,y) = 0;
            break;
        }
    }

    src.at<Vec3b>(x,y)[0] = new_color[0];
    src.at<Vec3b>(x,y)[1] = new_color[1];
    src.at<Vec3b>(x,y)[2] = new_color[2];

    forbidden.at<uchar>(x,y) = 1;

    ncolor = new_color;

    for (int i = 1; i <= NUMBER_OF_LABELS; i++) {
        if (ncolor == Scalar(color[i][2],color[i][1],color[i][0])) {
            labMask[i - 1].at<uchar>(x,y) = 255;
            break;
        }
    }
}

/**
 * Count the colors of the neighbors of pixel (x,y).
 * @param src Source image matrix.
 * @param x Coordinate x of the pixel.
 * @param y Coordinate y of the pixel.
 * @param col_c Color count array.
 */
void neighborhood_color(Mat& src, int x, int y, int *col_c) 
{
    Vec3b ncolor = src.at<Vec3b>(x,y);

    for (int i = 1; i <= NUMBER_OF_LABELS; i++) {
        if (Scalar(ncolor) == Scalar(color[i][0],
                                     color[i][1],
                                     color[i][2]))      
        {
            col_c[i - 1]++;
        }
    }
}



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
         vector<Mat>& labMask) 
{
    if (forbidden.at<uchar>(x,y) ) {
        return 0;
    }

    int nx, ny;
    static int dx[] = {-1,0,1,0};
    static int dy[] = {0,1,0,-1};

    queue<pair<int, int> > q;
    pair <int, int> elem;

    int size = 1;

    // change color
    if ( Scalar(src.at<Vec3b>(x,y)) == original_color ){
        assign_color(src,forbidden,x,y,new_color, labMask);
    } else {
        return 0;
    }


    // get neighbour pixels
    for (int index=0; index<4; index++) {
        nx = x + dx[index];
        ny = y + dy[index];

        if (0 <= nx && nx < src.rows
                && 0 <= ny && ny < src.cols 
                && (forbidden.at<uchar>(nx,ny) == 0)
                && (Scalar(src.at<Vec3b>(nx,ny)) == original_color))
        {
            assign_color(src,forbidden,nx,ny,new_color,labMask);
            q.push(pair<int, int>(nx, ny));
            size++;
        }
    }

    while (!q.empty()) {
        elem = q.front();
        q.pop();

        for (int index=0; index<4; index++) {
            nx = elem.first + dx[index];
            ny = elem.second + dy[index];

            if  (0 <= nx && nx < src.rows
                    && 0 <= ny && ny < src.cols
                    && (forbidden.at<uchar>(nx,ny) == 0) 
                    && (Scalar(src.at<Vec3b>(nx,ny)) == original_color))
            {
                assign_color(src,forbidden,nx,ny,new_color,labMask);
                q.push(pair<int, int>(nx, ny));
                size++;
            }
        }
    }

    return size;
}

/**
 * Forms a connected component from pixel (x,y).
 * @param src source image matrix.
 * @param v matrix of visited pixels.
 * @param x coordinate x of the pixel.
 * @param y coordinate y of the pixel.
 * @param col_c color count array.
 * @return size of the connected component.
 */
int flood_fill(Mat& src, Mat& v, int x, int y, int *col_c)
{
    
    int nx, ny;
    static int dx[] = {-1,0,1,0};
    static int dy[] = {0,1,0,-1};

    queue<pair<int, int> > q;
    pair <int, int> elem;

    v.at<uchar>(x,y) = 1;
    int size = 1;

    // get neighbour pixels
    for (int index=0; index<4; index++) {
        nx = x + dx[index];
        ny = y + dy[index];

        if (0 <= nx && nx < src.rows && 0 <= ny && ny < src.cols)
        {
            Scalar current_color(src.at<Vec3b>(x,y));
            Scalar neighbor_color(src.at<Vec3b>(nx,ny));

            if(current_color == neighbor_color
               && ( v.at<uchar>( nx, ny ) == 0 )) 
            {
                v.at<uchar>(nx,ny) = 1;
                q.push(pair<int, int>(nx, ny));
                size++;
            }

            if (current_color != neighbor_color) {
                neighborhood_color(src, nx, ny, col_c);
            }
        }
    }

    while (!q.empty()) {
        elem = q.front();
        q.pop();

        for (int index=0; index<4; index++) {
            nx = elem.first + dx[index];
            ny = elem.second + dy[index];

            if  (0 <= nx && nx < src.rows && 0 <= ny && ny < src.cols) 
            { 
                
                Scalar current_color(src.at<Vec3b>(elem.first,
                    elem.second));

                Scalar neighbor_color(src.at<Vec3b>(nx,ny));
   
                if(current_color == neighbor_color
                   && ( v.at<uchar>( nx, ny ) == 0)) 
                {
                    v.at<uchar>(nx,ny) = 1;
                    q.push(pair<int, int>(nx, ny));
                    size++;
                }
                if (current_color != neighbor_color) {
                    neighborhood_color(src, nx, ny, col_c);
                }
            }
        }
    }
   
    return size;
}


/** 
 * Procedure to mark pixels as forbiden.
 * @param src source image matrix.
 * @param f Matrix of forbidden pixels.
 * @param x coordinate x of the pixel.
 * @param y coordinate y of the pixel.
 * @param col_c color count array.
 */
void forbid(Mat& src, Mat& f, int x, int y, int col_c[])
{
    flood_fill(src, f, x, y, col_c);
}

/**
 * Selects the color for the connected component based on the cantity
 * of neighbour colors.
 * @param col_c Color count array.
 * @param new_col New color of the connected component.
 */
void select_new_color (int *col_c, Scalar& new_col) 
{
    int i;
    int index = -1;
    int max = -1;
    //float max = -1.0;

    // Calculate best color
    for (i = 0; i < 6; i++) {
        if (col_c[i] > max) {
            max = col_c[i];
            index = i;
        }
    }

    //Note: Mat es BGR
    for (i = 1; i <= NUMBER_OF_LABELS; i++) {
        if (i == (index + 1)) {
            new_col = Scalar(color[i][0],color[i][1],color[i][2]);
            break;
        }
    }
}

/**
 * BFS flood fill procedure.
 * @param src source image matrix.
 * @param labMask Probability mask per label.
 */
void flood_fill_filter(Mat& src, vector<Mat>& labMask, int threshold)
{
    Mat visited(src.size(), CV_8U, Scalar(0));
    Mat forbidden(src.size(), CV_8U, Scalar(0));

    int color_count[NUMBER_OF_LABELS];
    float prob_count[NUMBER_OF_LABELS];
    Scalar original_color;
    Scalar new_color;

    // Obtengo el tamaño de la componente conexa, si es mas pequeña
    // que el umbral, la pinto, sino, la prohibo
    for (int x = 0; x < src.rows; x++) {
        for (int y = 0; y < src.cols; y++) {
            if ( visited.at<uchar>(x, y) == 0 
                 && (src.at<Vec3b>(x,y)) != Vec3b(0,0,0)) {

                memset(color_count, 0, 6*sizeof(int));
                memset(prob_count, 0.0, 6*sizeof(float));

                int size = flood_fill(src, visited, x, y, color_count);

                if (size <= threshold) {
                    
                    original_color = Scalar(src.at<Vec3b>(x,y)[0],
                                            src.at<Vec3b>(x,y)[1],
                                            src.at<Vec3b>(x,y)[2]);

                    select_new_color (color_count, new_color);
                    
                    size = fill(src, 
                                forbidden, 
                                x, 
                                y, 
                                original_color, 
                                new_color, 
                                labMask);

                } 
                else {
                    forbid(src, forbidden, x, y, color_count);
                }
            }
        }
    }
}

/**
 * Flood fill procedure.
 * @param src source image matrix.
 * @param labMask Probability mask per label.
 * @param iter Number of iterations of flood fill.
 */
void flood_fill_proc (Mat& src, 
                      vector <Mat>& labMask, 
                      int iter, 
                      int threshold) 
{
    for (int i = 0; i < iter; i++) {
        flood_fill_filter(src, labMask, threshold);
    }
}

float flood_fill_proc_test (Mat& src, 
                            Mat& t_img,
                            vector <Mat>& labMask, 
                            int iter, 
                            int threshold) 
{

    //Pre - Classification
    int positive;
    int noNullPixels;
    float preclasif;
    float clasif;

    positive = 0;
    noNullPixels = 0;

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if (src.at<Vec3b>(i,j) != Vec3b(0,0,0)) {
                
                noNullPixels++;
                
                if (src.at<Vec3b>(i,j) == t_img.at<Vec3b>(i,j)) {
                    positive++;
                }
            }
        }
    }
    
    preclasif = (float)positive / (float)noNullPixels;
    cout << "preclasif: " << preclasif  << endl;


    for (int i = 0; i < iter; i++) {
        flood_fill_filter(src, labMask, threshold);
    }

    //Classification
    positive = 0;
    noNullPixels = 0;

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if (src.at<Vec3b>(i,j) != Vec3b(0,0,0)) {
                
                noNullPixels++;
                
                if (src.at<Vec3b>(i,j) == t_img.at<Vec3b>(i,j)) {
                    positive++;
                }
            }
        }
    }
    clasif = (float)positive / (float)noNullPixels;
    cout << "classif: " << clasif << endl;
    
    cout << "diff: " << clasif - preclasif << endl;
    return (float)positive / (float)noNullPixels;

}
