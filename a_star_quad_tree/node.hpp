// Node.hpp



#ifndef NODE
#define NODE

#include <stdio.h>
#include <stdlib.h>

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
	int resolution; //Largeur du node (1,2,4,8,16,32,...)
	Node parent_node;
	Node_type type;
	std::array<Node*, 4> sons; //liste des sous-arbres
	std::array<Node *, 4> neighborhood; //liste des voisins
	
	//Attributs for A* w/ quad trees :
	int weight_so_far;
	Node coming_from;
	int weight;

	
    public:
	Node(Node_type type, Vector<Node> neighborhood, Vector<Node> sons){
	    this->weigh_so_far=MAXINT;
	    this->parent_node = NULL;
	    this->type = type;
	    this->neighborhood = neighborhood;
	    this->sons = sons;
	}
	
	int heuristique(Node goal){
	    return abs(this->x - goal->x) + abs(this->y - goal->y); //Distance de manhattan
	}
	    

	

	
	
	
#endif

	//for latter : priority_queue<Node*, vector<Node*>, DereferenceCompareNode> queue
