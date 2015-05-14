#include <rdf/ImagePool.h>

/** \brief Constructor.
 *  Loads the image content of the specified directory.
 *  \param[in] dirname Directory path to the image pool.
 */
rdf::ImagePool::ImagePool(const std::string dirname) {
    int id = 0;
    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    TrainImage img;
    vector <TrainImage> temp;

    std::string fileName;
   
    pdir = opendir(dirname.c_str());

    if (pdir == NULL) {
        printf ("Error! directory not initialized correctly\n");
        exit(EXIT_FAILURE);
    } 
    while ((pent = readdir (pdir)))  {

        if (pent == NULL) {
            printf("Error: could not read directory\n");
        }       

        fileName = dirname + string (pent -> d_name);
        
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
}


/** \brief Reorder the pool given a array of index.
 *  \param[in] index_vector The vector with the indices
 */
void rdf::ImagePool::poolReorder(std::vector<int>& index_vector) {
    std::vector<TrainImage> temp;
    for (const auto index : index_vector) {
        temp.push_back(images[index]);
    }
    images = temp;
}


/** \brief this functions retrieves the pixel Label of an image given a
 *  PixelInfo object.
 *
 *  \param[in] pi The pixel identification.
 *  \return return The pixel label.
 */
Label rdf::ImagePool::getLabel(const PixelInfo& pi) {
    return images[pi.id].getLabel(pi.x, pi.y);
}


/** \brief this functions retrieves the pixel depth of an image given a
 *  PixelInfo object.
 *
 *  \param[in] pi The pixel identification.
 *  @return The pixel depth.
 */
unsigned rdf::ImagePool::getDepth(const PixelInfo& pi) {
    return images[pi.id].getDepth(pi.x, pi.y);
}


//CHECK
rdf::TrainImage* rdf::ImagePool::getImgPtr(unsigned i) {
    return &(images[i]);
}
