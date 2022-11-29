#ifndef SVDAG_H
#define SVDAG_H

#include <string>
#include <vector>
#include <algorithm>
#include "../svo_construction/Node.h"
#include "../svo_construction/octree_io.h"
#include "SVDAG_node.h"


class SVDAG 
{
public:
	SVDAG(const string& filename);

    void read_SVO(string filename); 			// Reading the octree from file 
	void write_SVDAG(); 						// Saving DAG in the file "filename.dag"

	void print_SVDAG_info() const; 				// Printing general stats on constructed DAG
	void print_level(size_t cur_level) const; 	// Printing all vertices on level L

	void reduce(); 								// The actual implementation of DAG reduction algo

private:
	vector<Node> octree_nodes; 					// All the SVO nodes read from the input files
	OctreeInfo info; 							// SVO info  
	vector<SVDAG_node> SVDAG_nodes; 			// All DAG vertices. 
												// Vertices that have been merged are not deleted, 
												// so that the index of the vertex in this vector is the id of the vertex

	vector<vector<size_t>> SVDAG_levels; 		// All DAG levels and the id of DAG vertices


	void construct_SVDAG(); 					 // Construct the unreduced DAG
	vector<size_t> construct_next_SVDAG_level(
				   const vector<size_t>& level); // Construct the next DAG level from the previous one
	void fill_SVDAG_parents(SVDAG_node& root);   // Fill the parents field of SVDAG_node
	void sort_level(size_t cur_level); 			 // Sort the DAG nodes on the level L
	void group_level(size_t cur_level);			 // Group identical DAG nodes (same childmask, same children pointers) 
												 // and updating parent nodes so that they point to the merged nodes
};

#endif //SVDAG_H
