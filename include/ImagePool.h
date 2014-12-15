# ifndef IMAGEPOOL_H
# define IMAGEPOOL_H

# include <RGBD-RF/common.hpp>
# include "Image.h"
# include "PixelInfo.h"

class ImagePool 
{
    private:


    public:
        
        vector <TrainImage> images;
        /** 
         * Class Constructor
         */
        ImagePool();
        
        ImagePool(string imgDir);

        ~ImagePool() {}

        /**
         * Reorder the pool given a array of
         * index.
         * @param idxVec vector of indexes.
         */
        void poolReorder(vector <int> idxVec);

        int poolSize ();

        /**
         *  getLabel
         *
         *  this functions retrieves the pixel Label of an image given a
         *  PixelInfo object.
         *
         *  @param pi is the pixel object.
         *  @return The pixel label.
         */
        Label getLabel(PixelInfo& pi);

        /**
         *  getDepth
         *
         *  this functions retrieves the pixel depth of an image given a
         *  PixelInfo object.
         *
         *  @param pi is the pixel object.
         *  @return The pixel depth.
         */
        unsigned getDepth(PixelInfo& pi);


        TrainImage* getImgPtr(unsigned i);

        TrainImage& operator[] (int i) { return images[i]; }
};

# endif
