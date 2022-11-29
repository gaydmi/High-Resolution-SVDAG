#include "SVDAG_node.h"

SVDAG_node::SVDAG_node(size_t id, Node n) 
{
    this->id = id;
    childmask = 0;
    for(unsigned j=0; j<8; j++)
    {
        childmask = (childmask << 1) + n.hasChild(j);
        if(n.hasChild(j)) children.push_back(n.getChildPos(j));
    }
}

// Operators overloading for sorting purposes
bool SVDAG_node::operator< (const SVDAG_node &n) const 
{   
    /*
    if(childmask!=n.childmask) return childmask < n.childmask;

    for(int j=0; j<children.size(); j++)
    {
        if(children[j]!=n.children[j])  return children[j] < n.children[j];
    }
    return false;
    */
    for (int i = 0; i < 8; i++) {
		if (children[i] == n.children[i]) continue;
		return children[i] < n.children[i];
	}
	return false;
}

bool SVDAG_node::operator==(const SVDAG_node &n) const 
{
    if(childmask!=n.childmask) return false;
    
    for(int j=0; j<children.size(); j++)
    {
        if(children[j]!=n.children[j]) return false;
    }
    return true;
}

// Util and debug methods 
void SVDAG_node::print()
{
    string s = "SVDAG_node " + std::to_string(id) + " - ";
    s += "childmask: " + std::to_string(childmask) + " (";
    
    int a = childmask;
    for(int j=7; j>=0; j--)
    {
        s+= a % int(pow(2, j)) == a ? "0" : "1";
        a = a % int(pow(2, j));
    }
    s += "); ";
    s += "children: ";
    for(size_t child: children)
    {
        s += std::to_string(child) + "; ";
    }
    s += "parents: ";
    for(size_t parent: parents)
    {
        s += std::to_string(parent) + "; ";
    }
    std::cout << s << std::endl;
}

void SVDAG_node::write_node(FILE *out) 
{
    fwrite(&childmask, sizeof(uint8_t), 1, out);
    if(!is_leaf()) fwrite(&children[0], sizeof(size_t), children.size(), out);
}

bool SVDAG_node::is_leaf() const 
{
    return children.empty();
}