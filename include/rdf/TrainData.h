#ifndef RGBD_RF_IMAGE_TRAIN_SET_HH__
#define RGBD_RF_IMAGE_TRAIN_SET_HH__

#include <memory>
#include <rdf/PixelInfo.h>
#include <rdf/ImagePool.h>

namespace rdf {

/** \brief Container for the training data.
 *
 *  The training data consist of a vector of pixels. These pixels can belong to 
 *  different images.
 */
class TrainData {
    public:
        typedef std::shared_ptr<TrainData> Ptr;

        /** \brief Default constructor. **/
        TrainData() = default;

        /** \brief .
         *  \param[in] num_imgs Number of images in the training set.
         *  \param[in] num_pixels Number of pixels per training image.
         */
        TrainData(int num_imgs, int num_pixels)
            : pixels(num_imgs * num_pixels) {}

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
        int size () { return static_cast<int>(pixels.size()); }
        
        PixelInfo& operator[] (int i) { return pixels[i]; }

        /** \brief Returns a pointer to the beginning of the pixel vector.
         *  \return Iterator to the beginning of the pixel vector.
         */
        std::vector<PixelInfo>::iterator begin() { return pixels.begin(); }

        /** \brief Returns a pointer to the end of the pixel vector.
         *  \return Iterator to the end of the pixel vector.
         */
        std::vector<PixelInfo>::iterator end() { return pixels.end(); }

    private:
        /* Pixels from image to train */
        std::vector<PixelInfo> pixels;
};

} // namespace rdf 

# endif // RGBD_RF_IMAGE_TRAIN_SET_HH__
