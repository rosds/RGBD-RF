# ifndef IMAGE_TRAIN_SET_H
# define IMAGE_TRAIN_SET_H

# include "common.h"
# include "PixelInfo.h"
# include "ImagePool.h"

class TrainData {

    private:
     
        /* Pixels from image to train */
        vector <PixelInfo> pixelC;

    public:


        TrainData ();

        /**
         * Creates an empty vector of trainImgNum x numPixels
         */
        TrainData (int trainImgNum, int numPixels);

        /**
         * Selects random pixels from the image imgId
         */
        TrainData (int numPixels, 
                   ImagePool& imgPool,
                   int startIdx,
                   int endIdx,
                   bool byLabel = false);

        /**
         * Returns the number of pixels
         */
        int size ();
        
        PixelInfo& operator[] (int i) { return pixelC[i]; }

};

# endif
