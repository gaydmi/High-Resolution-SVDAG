#ifndef SVDAG_NODE_H
#define SVDAG_NODE_H

#include <stdlib.h>
#include <vector>
#include <ostream>
#include <iostream>
#include <unordered_set>
#include "../svo_construction/Node.h"


// SVDAG node data structure
class SVDAG_node
{
public:
    SVDAG_node(size_t id, Node n);               // Constructor from octree node. Parents are empty.
    SVDAG_node(){}

    std::vector<size_t> children;                // Positions of child nodes
    std::unordered_set<size_t> parents;          // Positions of parent nodes
    size_t id;                                   // Index of the current node
    uint8_t childmask;                           // Children bitmask, '1' == there is a child

    bool operator< (const SVDAG_node &n) const;  // Compare childmask. If identical, use lexicographic order on children
    bool operator== (const SVDAG_node &n) const; // Same childmask, same children

    // Util and debug methods
    bool is_leaf() const;                        // Check if current node is a leaf
    void print();                                // Print SVDAG vertex with id, childmask, children and parents
    void write_node(FILE* out);                  // Write a node in a file
};


#endif //SVDAG_NODE_H
