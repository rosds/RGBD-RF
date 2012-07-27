/**
 *  @file Image.cpp
 *
 *  @brief This file contain the definition of the functions from the
 *  file Image.h
 */

# include "Image.h"

/**
 *  Image constructor. Loads the image representation to memory.
 *  @param filename Filename of the input image in sparse representation.
 */
TrainImage :: TrainImage (const string& fileName)
{
    Label l;
    unsigned s;
    unsigned d;
    FILE* fp;


    if((fp = fopen(fileName.c_str(), "r")) == NULL) {
        printf("Cannot open file %s.\n", fileName.c_str());
        exit(1);
    }

    // Read the image aspect ratio
    fscanf(fp, "%hd %hd;\n", &width, &height);

    // Read image pixel labels
    while ( fscanf(fp, "%d", &s) > 0 ) {
        l = s;
        pixelLabels.push_back(l);
    }
    fseek(fp, 1, SEEK_CUR);

    // Read image depth labels
    while ( fscanf(fp, "%d", &d) > 0 ) {
        pixelDepths.push_back(d);
    }
    fseek(fp, 1, SEEK_CUR);

    // Read the vector I info
    while ( fscanf(fp, "%d", &s) > 0 ) {
        I.push_back(s);
    }
    fseek(fp, 1, SEEK_CUR);

    // Read the vector J info
    while ( fscanf(fp, "%d", &s) > 0 ) {
        J.push_back(s);
    }
    fclose(fp);
}

/** 
 * Gets the index of the content array of the element in 
 * position (x,y).
 * @param x Pixel X-axis coordinate.
 * @param y Pixel Y-axis coordinate.
 * @return index of the content array, NOT_FOUND if pixel from 
 * background.
 */
// CHECK
int TrainImage :: getIndex(const short & x, const short & y)
{
    int index;

    vector <unsigned short> :: iterator it;
    vector <unsigned short> :: iterator begin;
    vector <unsigned short> :: iterator end;

    // Check the range of x
    if ((x < 0) || (x >= height)) {
        return NOT_FOUND;
    }
    // Check the range of y
    if ((y < 0) || (y >= width)) {
        return NOT_FOUND;
    }
    
    // If the 'I' vector in the position X is equal to 0, then there is
    // any element in that row and return the default value;
    if (I[x] == 0) {
        return NOT_FOUND;
    }

    // Set the index to start searching for y
    if (x == 0) {
        begin = J.begin();
    }
    else {
        begin = J.begin() + I[x - 1];
    }

    end = J.begin() + I[x] - 1;
    
    it = lower_bound (begin, end, y);
    index = it - J.begin();

    if (J[index] == y) {
        return index;
    }

    return NOT_FOUND;
}

/**
 * Gets the label value of the pixel in position (x,y). 
 * @param x Pixel X-axis coordinate.
 * @param y Pixel Y-axis coordinate.
 * @return Label value of the pixel (x,y) or DEFAULT_LABEL if
 * label not found.
 */
//CHECK
Label TrainImage :: getLabel(const short & x, const short & y)
{
    int index;
    index = getIndex(x,y);
    if (index != NOT_FOUND) {
        return pixelLabels[index];
    }
    else {
        return DEFAULT_LABEL;
    }
}

/**
 * Gets the depth value of the pixel in position (x,y). 
 * @param x Pixel X-axis coordinate.
 * @param y Pixel Y-axis coordinate.
 * @return depth value of the pixel (x,y) or DEFAULT_DEPTH if
 * depth not found.
 */
//CHECK
unsigned TrainImage :: getDepth(const short & x, const short & y)
{
    int index;
    
    index = getIndex(x,y);
    if (index != NOT_FOUND) {
        return pixelDepths[index];
    }
    else {
        return DEFAULT_DEPTH;
    }
}

/**
 *  getRandomCoord
 *
 *  This function get a random coordinate.
 *  @return a random coordinate of pixel.
 */
//CHECK
Coord TrainImage :: getRandomCoord()
{
    unsigned ind;
    unsigned x;
    unsigned short y;
    vector <unsigned> :: iterator it;

    ind = rand() % J.size();

    // get the col number
    y = J[ind];
    
    it = upper_bound (I.begin(), I.end(), ind);
    
    x = int(it - I.begin());

    return Coord (x, y);
}

/**
 *  getRandCoordByLabel
 *
 *  This function get a random coord of a given label group of
 *  pixels.
 *
 *  @param type of label.
 *
 *  @return a random coordinate of pixel of the given label.
 */
void TrainImage :: getRandCoordByLabel(int pixNum, vector<PixelInfo>& p, int imgId)
{
    unsigned i;
    unsigned j;
    unsigned ind;
    unsigned x;
    unsigned short y;
    vector <unsigned> Jind[NUMBER_OF_LABELS];
    vector <bool> labImg(NUMBER_OF_LABELS, false);
    vector <unsigned> :: iterator it;
    int labFound = 0;
    unsigned pixPerLab;
    unsigned rest;
    unsigned pixToTake;
 
    p.clear();

    // Find all the label index and organize them in Jind matrix
    for (i = 0; i < pixelLabels.size(); i++) {
        if (labImg[pixelLabels[i] - 1] == false) {
            labImg[pixelLabels[i] - 1] = true;
            labFound++;
        }
        Jind[pixelLabels[i] - 1].push_back(i);
    }

    pixPerLab = pixNum / labFound;

    rest = pixNum - (pixPerLab * labFound);

    for (i = 0; i < NUMBER_OF_LABELS; i++) {
        if (labImg[i] == true) {
            if (rest > 0) {
                pixToTake = pixPerLab + 1;
                rest--;
            }
            else {
                pixToTake = pixPerLab;
            }
            for (j = 0; j < pixToTake; j++) {
                // Get random index from the label group
                ind = Jind[i][ rand() % Jind[i].size() ];

                // get the col number
                y = J[ind];
                
                it = upper_bound (I.begin(), I.end(), ind);

                x = int(it - I.begin());

                p.push_back(PixelInfo(Coord(x, y), imgId));
            }
        }
    }
}

/**
 * Prints the content labels of the entire train image.
 * @param width Width of the image.
 * @param height Height of the image.
 */
 //CHECK
void TrainImage :: print (int width, int height) 
{
    for (int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf ("%d ", getLabel(i,j));
        }
        printf("\n");
    }
}

/**
 * Gets the depth value of the element in position (x,y).
 * @param x Pixel X-axis coordinate.
 * @param y Pixel Y-axis coordinate.
 * @return Depth value of the (x,y) pixel.
 */
unsigned KinectImage :: getDepth(const short & x, const short & y)
{
    // Check the range of x
    if ((x < 0) || (x >= height)) {
        return DEFAULT_DEPTH;
    }
    // Check the range of y
    if ((y < 0) || (y >= width)) {
        return DEFAULT_DEPTH;
    }

    unsigned d = depth[(width * x) + y];

    // Filter kinect null depth values
    if (d == 0) {
        return DEFAULT_DEPTH;
    }
    return d;
}
