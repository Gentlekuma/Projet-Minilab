// Node.hpp

// Class for quadtree used with A* algorithm v0.1.1

#ifndef NODE
#define NODE

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <functional>
struct PQCompareNode : public std::binary_function<Node*, Node*, bool>
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return lhs->weight() > rhs->weight();
    }
};

enum Node_type {blocked, mixed, free, goal};

class Node
{
    protected:
	//Attributs from the map :
	int x;
	int y;
	int resolution; //node size in pixels (1,2,4,8,16,32,...)
	Node parent_node;
	Node_type type;
	std::array<Node*, 4> sons; //subtrees list
	std::array<Node *, 4> neighborhood; //adjacent nodes list (4 nodes N,W,S,E) 
	
	//Attributs for A* w/ quad trees :
	int weight_so_far;
	Node coming_from;
	int weight;

    public:
	Node(int x, int y, int resolution, Node parent_node, Node_type type, std::array<Node*, 4> sons, std::array<Node* ,4> neighborhood){
	    this->weigh_so_far=INT_MAX;
	    this->parent_node = NULL;
	    this->type = type;
	    this->neighborhood = neighborhood;
	    this->sons = sons;
	}
	
	int heuristique(Node goal){
	    return abs(this->x - goal->x) + abs(this->y - goal->y); //Distance de manhattan
	}
	    
	bool isLeaf(){
	    return false;
	}
	

	
	
	
#endif
	//use direction for cost (cost higher if changing direction and for quad trees)
	//for latter : priority_queue<Node*, vector<Node*>, DereferenceCompareNode> queue
