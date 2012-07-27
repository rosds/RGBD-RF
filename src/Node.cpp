# include "Node.h"

Node :: Node() {
    id = 0;
}

SplitNode :: SplitNode ()
{
    up = NULL;
    left = NULL;
    right = NULL;
}

SplitNode :: SplitNode (SplitCandidate p)
{
    up = NULL;
    left = NULL;
    right = NULL;
    phi = p;
}

bool SplitNode :: nodeType () 
{
    return SPLIT;
}

bool LeafNode :: nodeType () 
{
    return LEAF;
}


LeafNode :: LeafNode ()
{
    pDist = vector <float> ();
}

LeafNode :: LeafNode (vector <float> prob)
{
    pDist = prob;
}
