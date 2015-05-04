/**
 *  @file Image.cpp
 *
 *  @brief This file contain the definition of the functions from the
 *  file Image.h
 */

# include "Image.h"


/** \brief Loads the image representation to memory.
 *  \param[in] filename Filename of the input image in sparse representation.
 */
rdf::TrainImage::TrainImage(const std::string& fileName) {
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


/** \brief Gets the index of the content array of the element in 
 * position (x,y).
 * \param[in] x Pixel X-axis coordinate.
 * \param[in] y Pixel Y-axis coordinate.
 * \return index of the content array, NOT_FOUND if pixel from 
 * background.
 */
int rdf::TrainImage::getIndex(const short& x, const short& y) {
    int index;

    std::vector<unsigned short>::iterator it;
    std::vector<unsigned short>::iterator begin;
    std::vector<unsigned short>::iterator end;

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

/** \brief Gets the label value of the pixel in position (x,y). 
 * \param[in] x Pixel X-axis coordinate.
 * \param[in] y Pixel Y-axis coordinate.
 * \return Label value of the pixel (x,y) or DEFAULT_LABEL if
 * label not found.
 */
Label rdf::TrainImage::getLabel(const short& x, const short& y) {
    int index;
    index = getIndex(x,y);
    if (index != NOT_FOUND) {
        return pixelLabels[index];
    }
    else {
        return DEFAULT_LABEL;
    }
}


/** \brief Gets the depth value of the pixel in position (x,y). 
 * \param[in] x Pixel X-axis coordinate.
 * \param[in] y Pixel Y-axis coordinate.
 * \return depth value of the pixel (x,y) or DEFAULT_DEPTH if
 * depth not found.
 */
unsigned rdf::TrainImage::getDepth(const short& x, const short& y) {
    int index;
    
    index = getIndex(x,y);
    if (index != NOT_FOUND) {
        return pixelDepths[index];
    }
    else {
        return DEFAULT_DEPTH;
    }
}


/** \brief Sample a random non-zero element from a Yale representation of a sparse 
 *  matrix.
 *  
 *  \param[out] row Row of the element.
 *  \param[out] col Col of the element.
 */
void rdf::TrainImage::getRandomCoord(uint32_t& row, uint32_t& col) {
    // pick a random non-zero element.
    const auto ind = static_cast<uint32_t>(rand() % J.size());

    // get the col.
    col = J[ind];
    
    // get the row.
    const auto& it = std::upper_bound(I.begin(), I.end(), ind);
    row = static_cast<uint32_t>(it - I.begin());
}

/** \brief This function get a random coord of a given label group of
 *  pixels.
 *
 *  \param[in] type of label.
 *  \return a random coordinate of pixel of the given label.
 */
void rdf::TrainImage::getRandCoordByLabel(int pixNum, std::vector<PixelInfo>& p, int imgId) {
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

                p.push_back(PixelInfo(x, y, imgId));
            }
        }
    }
}

/** \brief Prints the content labels of the entire train image.
 * \param[in] width Width of the image.
 * \param[in] height Height of the image.
 */
void rdf::TrainImage::print(const int width, const int height) {
    for (int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf ("%d ", getLabel(i,j));
        }
        printf("\n");
    }
}

/** \brief Gets the depth value of the element in position (x,y).
 *  \param[in] x Pixel X-axis coordinate.
 *  \return Depth value of the (x,y) pixel.
 *  \param[in] y Pixel Y-axis coordinate.
 */
unsigned rdf::KinectImage::getDepth(const short& x, const short& y)
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
