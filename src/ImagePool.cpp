# include "ImagePool.h"

/**
 * Empty class constructor.
 */
ImagePool :: ImagePool () 
{
    images = vector <TrainImage> ();
}

/**
 * Class constructor.
 * Loads the image content of the specified directory.
 * @param imgDir Pool image directory.
 */
 //CHECK
ImagePool :: ImagePool (string imgDir)
{
    int id = 0;
    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    TrainImage img;
    vector <TrainImage> temp;

    string fileName;
   
    pdir = opendir (imgDir.c_str());

    if (pdir == NULL) {
        printf ("Error! directory not initialized correctly\n");
        exit(EXIT_FAILURE);
    } 
    while ((pent = readdir (pdir)))  {

        if (pent == NULL) {
            printf("Error: could not read directory\n");
        }       

        fileName = imgDir + string (pent -> d_name);
        
        // Checking extension
        if (fileName.substr(fileName.size() - 5, 5) == ".simg") {
            printf("Image %s loaded\n", fileName.c_str());

            // Initialize train image structure
            img = TrainImage(fileName.c_str());
            img.id = id;
            // insert into pool
            images.push_back(img);
            id++;
        }
        else {
            printf("File %s not loaded: Bad extension\n", fileName.c_str());
        }
    }
    
/*
    // Disorder images array
    while (temp.size() != 0) {
        idx = rand() % temp.size();
        images.push_back(temp[idx]);
        temp.erase(temp.begin() + idx);
    }*/
}

/**
 * Reorder the pool given a array of
 * index.
 * @param idxVec vector of indexes.
 */
void ImagePool :: poolReorder(vector <int> idxVec) 
{
    unsigned i;
    vector <TrainImage> temp;
    
    for (i = 0; i < idxVec.size(); i++) {
        temp.push_back(images[idxVec[i]]);
    }
    images = temp;
}


/**
 * Returns the pool size.
 * @return Number of images in the pool.
 */
 //CHECK
int ImagePool :: poolSize () 
{
    return images.size();
}

/**
 *  getLabel
 *
 *  this functions retrieves the pixel Label of an image given a
 *  PixelInfo object.
 *
 *  @param pi is the pixel object.
 *  @return The pixel label.
 */
 //CHECK
Label ImagePool :: getLabel(PixelInfo& pi)
{
    return images[pi.id].getLabel(pi.x, pi.y);
}

/**
 *  getDepth
 *
 *  this functions retrieves the pixel depth of an image given a
 *  PixelInfo object.
 *
 *  @param pi is the pixel object.
 *  @return The pixel depth.
 */
 //CHECK
unsigned ImagePool :: getDepth(PixelInfo& pi)
{
    return images[pi.id].getDepth(pi.x, pi.y);
}

//CHECK
TrainImage* ImagePool :: getImgPtr(unsigned i)
{
    return &(images[i]);
}
