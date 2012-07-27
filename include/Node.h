# ifndef NODE_H
# define NODE_H

# include "common.h"
# include "SplitCandidate.h"

using namespace std;

enum nodeType {
    SPLIT  = 0,
    LEAF = 1
};

       
class Node {

    public:
        int id;
    
        Node ();
        ~Node () {}

        virtual bool nodeType() = 0;
};

class SplitNode : public Node {

    public:

        Node *up;
        Node *left;
        Node *right;
        SplitCandidate phi;

        SplitNode();
        SplitNode(SplitCandidate p);
        ~SplitNode() {}

        bool nodeType ();

};

class LeafNode : public Node {

    public: 
        
        vector <float> pDist;

        LeafNode();
        LeafNode(vector <float> prob);
        ~LeafNode() {}
    
        bool nodeType ();
};

# endif
