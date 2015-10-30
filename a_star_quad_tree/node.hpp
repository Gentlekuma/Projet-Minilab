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

enum Node_type {BLOCKED_NODE, MIXED_NODE, FREE_NODE, GOAL_NODE};

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
	std::list<Node *> neighborhood; //adjacent nodes list (4 nodes N,W,S,E) 
	
	//Attributs for A* w/ quad trees :
	int weight_so_far; //Weight to go to this tree
	Node coming_from;
	
    public:
	Node(int x, int y, int resolution, Node parent_node, Node_type type, std::array<Node*, 4> sons, std::list<Node*> neighborhood){
	    this->x = x;
	    this->y = y;
	    this->resolution = resolution;
	    this->parent_node = parent_node;
	    this->type = type;
	    this->sons = sons;
	    this->neighborhood = neighborhood;
	    
	    //Default values :
	    this->weigh_so_far=INT_MAX;
	    this->coming_from=NULL;
	}
	
	int heuristique(Node goal){
	    return abs(this->x - goal->x) + abs(this->y - goal->y); //Distance de manhattan
	}
	
	bool isLeaf(){
	    return sons[0]==NULL && sons[1]==NULL && sons[2]==NULL && sons[3]==NULL;
	}
	
	bool isRoot(){
	    return parent_node == NULL;
	}

	bool isGoal(){
	    return type==GOAL_NODE;
	}
     
#endif
