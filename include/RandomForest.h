#ifndef RGBD_RF_RANDOM_FOREST_HH__
#define RGBD_RF_RANDOM_FOREST_HH__

#include <SplitCandidate.h>
#include <TrainData.h>
#include <rdf/common.h>
#include <rdf/Image.h>
#include <rdf/PixelInfo.h>
#include <rdf/Node.h>
#include <rdf/Offset.h>

namespace rdf {

class RandomForest;

enum pixelSet {
    LEFT  = 0,
    RIGHT = 1
};

/**
 *  @class trainParams
 *
 *  This structure is used to specify all the training parameters of the
 *  random forest.
 *  
 *  @param treeNum is the number of trees in the forest.
 *  @param labelNum is the number of labels in the classification
 *  images.
 *  @param imgNum is the total number of images for the training.
 *  @param imgDir is the path to the directory with the images.
 *  @param maxDepth is the maximum depth that can grow a tree.
 *  @param minSampleCount is the minimum number of exaples that can be
 *  splited in two leaf nodes.
 *  @param samplePixelNum is the number of pixels to be chosen randomly
 *  form a single image.
 *  @param trainImgNum is the number of images to be chosen randomly.
 *  @param offsetNum is the number of offsets to be generated for each
 *  node.
 *  @param thresholdNum is the number of threshodls to be generated for
 *  each node.
 *  @param offsetRange is the range of values that can be generated for
 *  the offsets.
 *  @param thresholdRange is the range of values that can be generated 
 *  for the thresholds.
 */
class  trainParams {
    public:
            int treeNum;
            int labelNum;
            int imgNum;
            string imgDir;
            int maxDepth; 
            int minSampleCount;
            int samplePixelNum;
            int trainImgNum; 
            int offsetNum; 
            int thresholdNum;
            NumRange offsetRange;
            NumRange thresholdRange;

            trainParams() {};
};

/**
 *  @struct SCParams
 *
 *  This structure is used for the function bestSplitCandidate to
 *  generate a Split Canditate to separate the train data examples.
 *
 */
struct SCParams {
    RandomForest* forest;
    NumRange trainDataRange;
};

void* findSplitThread (void* args);


class RandomForest {
    private:
      
        // Training parameters
        trainParams* tp;

        /* Images */
        ImagePool* images;

        /* Train data */
        TrainData* td;

        /* Array of the trees of the forest */
        vector <Node *> trees;

        /**
         *  calcSetEntropy
         *
         *  This function calculate the entropy of the train set data in
         *  the range given.
         *
         *  @parma range of the train data. 
         *
         *  @return the shannon entropy of the train data set between
         *  the begin and end index parameters.
         *
         */
        float calcSetEntropy(NumRange& setRange);

        /**
         *  Information Gain
         *
         *  This function returns the total gain of information 
         *  resulting after spliting the data set by the feature
         *  phi between the begin and end index parameters.
         *
         *  @param phi is the SplitCandidate to evalute its entropy
         *  @param setEntropy is the total entropy of the set
         *  @param range of the train set.
         *
         *  @return the total entropy of the phi feature
         *
         */

        float G(SplitCandidate phi,
                float setEntropy,
                NumRange setRange);

        /**
         *  getPercentage
         *
         *  This function apply the SplitCandidate phi to the TrainData set 
         *  between the begin and end index parameters. The percentage of
         *  the labels ocurrences are divided in two vector left and 
         *  right. Each of this vectors contains the percentage of each 
         *  label in each set.
         *  
         *  @param phi is the feature to be applyed to the trainData set.
         *  @param left is a vector that will contain the percentage of
         *  each label in the left splited set by phi.
         *  @param right is a vector that will contain the percentage of
         *  eachlabel in the right splited set by phi.
         *  @param leftCard is the cardinality of the resulting left
         *  set.
         *  @param rightCard is the cardinality of the resulting right
         *  set.
         *  @param range of the train set.
         *
         */
        void getPercentage(SplitCandidate phi,
                           vector <float>& left,
                           vector <float>& right,
                           unsigned& leftCard,
                           unsigned& rightCard,
                           NumRange range);
 
        /**
         *  Shannon Entropy function
         *
         *  @param percentage is a vector that contains the percentage
         *  of each label occurrence in a set.
         *
         *  @return the entropy associated with this percentages.
         */
        float H(const std::vector<float>& percentage);

        /**
         *  This function obtain the probability distribution of the
         *  labels in a range given of the train data pixels.
         *
         *  @param the range of pixels
         *  @param the train data set of pixels.
         *
         *  @return a vector with the propabilities
         */
        std::vector<float> probDist (NumRange setRange);

        /**
         *  This function determines if the given node stays as a leaf
         *  node or must be spliten y the training.
         *
         *  @param pointer to the node.
         *  @param range of the set.
         *  @param depth of the node.
         *  @param minimum number of examples per tree
         *
         *  @return true if the node must be a leaf or false in other case.
         */
        bool testNode (Node **n, 
                       Node *currentNode,
                       NumRange range);

        /**
         *  Gets the depth of a given node of a tree by the father
         *  chain.
         *
         *  @param node pointer
         *
         *  @return depth of the specified node
         */
        int getDepth (Node *n);
        
        /** \brief Calculates the feature function given the offsets and the
         *  pixel.
         * 
         *  \param[in] u first pixel offset.
         *  \param[in] v second pixel offset.
         *  \param[in] x Pixel coordinate of the pixel for the feature to be
         *  calculated.
         * 
         *  \return value of the calculated feature.
         */
        float calcFeature(
            const Offset& u, 
            const Offset& v,
            const PixelInfo& pi,
            Image *img
        );

        /** 
         * Sorts the training data array and returns the index
         * which splits the  array into left and right sets.
         *
         * @param range of the train set.
         * @param f feature that corresponds to the best split of the
         * training data
         *
         * @return index that defines the best split of the array.
         */
        int sortData (NumRange range, SplitCandidate f);
     
        /**
         *  classifyPixel
         *
         *  This function calculate the feature phi on the pixel x and
         *  depending on its threshold it will classify it as LEFT or
         *  RIGHT.
         *
         *  @param phi is te feacture to classify pixel.
         *  @param x is the pixel to be classifyed.
         *
         *  @return LEFT or RIGHT depending on the classification of the
         *  pixel x by the feature phi.
         */
        pixelSet classifyPixel(SplitCandidate phi, PixelInfo x, Image *img); 

         /**
         *  This function run the training of a single tree.
         *
         *  @param treeID of the tree to construct.
         *  @param maximum depth of the tree to construct.
         *  @param maximun number of pixels in a leaf node.
         *  @param number of pixels to select.
         *  @param number of training images.
         *  @param reference to the train data.
         */
        void train (int treeID);

        /**
         *  writeNodeToFile
         *
         *  This function write the information of a node to a file.
         *
         *  @param pointer to the node.
         *  @param file pointer to the file.
         */
        void writeNodeToFile (Node *currentNode, FILE *fp);

        /**
         *  writeTreeToFile
         *
         *  Write a trained tree to a file.
         * 
         *  @param treeID is the id of the trained tree to write into a
         *  file.
         *  @param fileName is the string containing the path to the
         *  output file.
         *
         */
        void writeTreeToFile(int treeID, string fileName);
        
        /**
         *  loadNodeFromFile
         *
         *  This function loads the information of a node.
         *
         *  @param pointer to the node.
         *  @param file pointer to the file.
         */
        void loadNodeFromFile (Node **currentNode,
                               Node **sideNode,
                               char nodeType,
                               int nodeID,
                               int side,
                               std::stack<Node*> *nStack,
                               FILE *fp);

        /**
         *  loadTreeToFile
         *
         *  load a trained tree from a file.
         * 
         *  @param fileName is the string containing the path to the file.
         */
        void loadTreeFromFile(const std::string& filename);

    public:

        /** \brief Constructor */
        RandomForest () : tp(nullptr) {}

        /**
         *  TODO: implement the destructor
         */
        virtual ~RandomForest () {}
 
        /**
         *  bestSplitThreadFun
         *
         *  This function runs the bestSplitCandidate in diferent
         *  threads. It divides the work between them by the macro
         *  THREADS_PER_NODE.
         *
         *  @param structure with the parameter to generate a split
         *  candidate.
         *
         *  @return The best split candidate generated
         */
        SplitCandidate bestSplitThreadFun(NumRange range);

        /**
         *  bestSplitCandidate
         *
         *  This function generate and return a feature that maximise
         *  the function G. This function create split candidates
         *  randomly and test them in the train data. The best feature
         *  generated will be return.
         *
         *  @param a structure with the information to generate a split
         *  candidate.
         *
         *  @return the best split candidate generated.
         */
        SplitCandidate bestSplitCandidate(SCParams& params);

        /**
         *  This fuction visit all the nodes in a tree specified and
         *  print its content.
         *
         *  @param id of the tree.
         */
        void traversal(int treeID);

        /**
         *  This function classify a pixel of a given image by the
         *  random forest.
         *
         *  @param pointer to the image.
         *  @param pixel to classofy
         *  @param probability of the classification.
         *  @return label of the classification
         */
        Label predict (Image* img, PixelInfo pixel, float& prob);

        /**
         *  This function start the training of the forest.
         *
         *  @param maximum depth of the tree.
         *  @param maximum number of examples to build a leaf node.
         *  @param number of pixels of an image for the training set.
         *  @param number of images per tree.
         *  @param number of offsets to generate.
         *  @param number of thresholds to generate.
         *
         */
        void trainForest (trainParams& tparams);

        /**
         *  Write the traided trees to diferent text files in a
         *  directory especified. The trees are saved in files named
         *  "i-Tree.txt".
         *
         *  @param path to the directory
         */
        void writeForest(const std::string& dirName);

        /**
         *  Load the trees contained in a directori with the name
         *  "i-Tree.txt".
         *
         *  @param path to the directory
         */
        void loadForest(int numTrees, int numLabels, const std::string& dirName);

        /**
         *  Return the percentage of classification of an image.
         *
         *  @param image to classify
         *  @return pecentage of pixels classified correctly
         */
        float testClassification (TrainImage& img);


        /**
         * Determines the percentage of pixels classified of an image and
         * print the classification to a image output file.
         * @param img Input image.
         * @return Percentage of pixels correctly classified.
         */
         //CHECK
        float testClassificationImage(TrainImage& img, const std::string& imgName);

};

} // namespace rdf

#endif // RGBD_RF_RANDOM_FOREST_HH__
