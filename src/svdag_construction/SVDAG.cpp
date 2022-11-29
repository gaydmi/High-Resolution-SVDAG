#include "SVDAG.h"


SVDAG::SVDAG(const string& filename) 
{
    read_SVO(filename);
    construct_SVDAG();
}

void SVDAG::read_SVO(string filename)
{
    // Reading Octree info
    parseOctreeHeader(filename + ".octree", info);
    if(info.filesExist())
    {
        info.print();
    } else 
    {
        std::cerr << "*.octree files don't exist. Please, check that ./svo_constructor have constructed those files." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Reading Octree Nodes info
    FILE * file = fopen(string(filename + ".octreenodes").c_str(), "r");
    if(file == NULL)
    {
        perror("Error opening file");
    } else 
    {
        int i = 0;
        while(!feof(file))
        {
            Node n;
            readNode(file, n);
            octree_nodes.push_back(n);
            i++;
        }
        std::cout << "File is processed. All nodes are in memory" << std::endl;
    }
}

void SVDAG::construct_SVDAG() 
{
    // Fill the nodes
    for(size_t i=0; i<octree_nodes.size(); i++)
    {
        SVDAG_node SVDAG_node(i, octree_nodes[i]);
        SVDAG_nodes.push_back(SVDAG_node);
    }
    // Fill parents field
    SVDAG_node root = SVDAG_nodes[SVDAG_nodes.size()-2]; // Root is at this position by construction of the file .octreenode
    fill_SVDAG_parents(root);

    // Fill SVDAG_levels 
    SVDAG_levels.push_back({SVDAG_nodes.size()-2});
    unsigned long cur_level = SVDAG_levels.size();
    vector<size_t> next_level = construct_next_SVDAG_level(SVDAG_levels[cur_level-1]);
    while(!next_level.empty())
    {
        SVDAG_levels.push_back(next_level);
        cur_level++;
        next_level = construct_next_SVDAG_level(SVDAG_levels[cur_level-1]);
    }
}

// Recursive implementation
void SVDAG::fill_SVDAG_parents(SVDAG_node &root) 
{
    for(size_t child: root.children)
    {
        SVDAG_nodes[child].parents.insert(root.id);
        fill_SVDAG_parents(SVDAG_nodes[child]);
    }
}

vector<size_t> SVDAG::construct_next_SVDAG_level(const vector<size_t> &level) 
{
    vector<size_t> next_level; // rests empty for the leaves
    for(const size_t& n_id: level)
    {
        for(size_t child: SVDAG_nodes[n_id].children)
        {
            next_level.push_back(child);
        }
    }
    return next_level;
}


// Reducing SVO to DAG
void SVDAG::reduce() 
{
    for(size_t cur_level=SVDAG_levels.size()-1; cur_level>=0 
                                                    && cur_level<SVDAG_levels.size(); cur_level--)
    {
        sort_level(cur_level);
        group_level(cur_level);
    }
}


void SVDAG::sort_level(size_t cur_level) 
{
    vector<size_t> level = SVDAG_levels[cur_level];
    std::sort(level.begin(), level.end());
    SVDAG_levels[cur_level] = level;
}

void SVDAG::group_level(size_t cur_level) 
{
    // Initialize new level
    vector<size_t> tmp_level = {SVDAG_levels[cur_level][0]};
    size_t group_id = tmp_level[tmp_level.size()-1];

    // Assert that the level is sorted
    for(int i=1; i<SVDAG_levels[cur_level].size(); i++)
    {   
        size_t n_id = SVDAG_levels[cur_level][i];
        if(SVDAG_nodes[n_id] == SVDAG_nodes[group_id])
        {   // Regrouping the nodes
            for(size_t parent: SVDAG_nodes[n_id].parents)
            {
                // Update the parents field of the regrouped node
                SVDAG_nodes[group_id].parents.insert(parent);
                // Update the children field of the parent node
                for(size_t& child: SVDAG_nodes[parent].children)
                {
                    if(child == n_id)
                        child = group_id;
                }
            }
        } else 
        { 
            tmp_level.push_back(n_id);
        }
    }

    // Replace the old level by the new level
    SVDAG_levels[cur_level] = tmp_level;
}


// Util methods for printing and writing to the file
void SVDAG::write_SVDAG() 
{
    FILE* file = fopen(string(info.base_filename + ".dag").c_str(), "wb");

    for(size_t cur_level=0; cur_level<SVDAG_levels.size(); cur_level++)
    {
        for(size_t i=0; i<SVDAG_levels[cur_level].size(); i++)
        {
            size_t n_id = SVDAG_levels[cur_level][i];
            SVDAG_nodes[n_id].write_node(file);
        }
    }
    fclose(file);
}

void SVDAG::print_SVDAG_info() const
{
    std::cout << "Counting SVDAG nodes..." << std::endl;
    int n = 0;
    for(int cur_level=0; cur_level<SVDAG_levels.size(); cur_level++)
    {
        n += SVDAG_levels[cur_level].size();
    }
    std::cout << "There are " << n << " SVDAG nodes, against " << info.n_nodes << " in SVO." << std::endl;
}

void SVDAG::print_level(size_t cur_level) const 
{
    std::cout << "Level " << cur_level << std::endl;
    for(size_t n_id: SVDAG_levels[cur_level])
    {
        SVDAG_node n = SVDAG_nodes[n_id];
        n.print();
    }
    std::cout << "There are " << SVDAG_levels[cur_level].size() 
              << " nodes at level " << cur_level << std::endl;
}


