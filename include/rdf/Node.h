#ifndef RGBD_RF_NODE_HH__
#define RGBD_RF_NODE_HH__

#include <vector>
#include <memory>

#include <SplitCandidate.h>

namespace rdf {

enum nodeType {
    SPLIT  = 0,
    LEAF = 1
};

/** \brief Base class for the node type.
 */
class Node {
    public:
        int id;
    
        Node () : id(0) {}

        /** \brief Destructor. **/
        virtual ~Node () {}

        virtual bool nodeType() = 0;
};


/** \brief Banch node containing the split candidate.
 *
 *  The split candidate determines whether the left or the right child should 
 *  be traversei.
 */
class SplitNode : public Node {
    public:

        SplitNode()
            : parent_(nullptr)
            , left_(nullptr)
            , right_(nullptr) {}

        SplitNode(const rdf::SplitCandidate& p)
            : parent_(nullptr)
            , left_(nullptr)
            , right_(nullptr)
            , phi(p) {}

        virtual ~SplitNode() {}

        bool nodeType() { return SPLIT; }

        SplitCandidate phi;

        Node* parent_;
        Node* left_;
        Node* right_;
};


/** \brief The leaf nodes contain a vector with the learned probability 
 * distribution of the classes after training.
 */
class LeafNode : public Node {
    public: 
        LeafNode() {}

        LeafNode(const std::vector<float>& prob)
            : pDist(prob) {}

        virtual ~LeafNode() {}
    
        bool nodeType() { return LEAF; }

        std::vector<float> pDist;
};

} // namespace rdf

# endif // RGBD_RF_NODE_HH__
