# ifndef RGBD_RF_IMAGE_TRAIN_SET_HH__
# define RGBD_RF_IMAGE_TRAIN_SET_HH__

# include "PixelInfo.h"
# include "ImagePool.h"

namespace rdf {

/** \brief Basically contains th
 *
 */
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

        /** \brief Returns the number of pixels
         *  \return The number of pixels.
         */
        int size ();
        
        PixelInfo& operator[] (int i) { return pixelC[i]; }

};

} // namespace rdf 

# endif // RGBD_RF_IMAGE_TRAIN_SET_HH__
