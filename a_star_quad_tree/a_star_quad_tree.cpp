#ifndef A_STAR_QT
#define A_STAR_QT

#include <queue>
#include <vector>
#include "a_star_quad_tree.hpp"
#include "node.hpp"
#include <iostream>

class Compare{
    public:
	bool operator()(const Node* lhs, const Node* rhs){
	    return lhs->weight_so_far > rhs->weight_so_far;
	}
};


void a_star_qt(Node *beginning, Node *goal, std::list<Node *> * path){
    std::priority_queue<Node*, std::vector<Node*>, Compare> frontier;
    Node * frontier_top;
    int new_weigh;
    Node* temp;

    //Explore the first node :

    beginning->weight_so_far=beginning->resolution;
       
    for (std::list<Node *>::iterator it=beginning->neighborhood.begin(); it != beginning->neighborhood.end(); ++it){
	(*it)->weight_so_far = beginning->weight_so_far + (*it)->resolution + goal->heuristique((*it));
	(*it)->coming_from = beginning;
	
	frontier.push((*it));
    }
    
    while(!frontier.empty() && !(frontier.top()->isGoal()) ){ 
	//while frontier not empty and goal not found

	frontier_top = frontier.top();
	frontier.pop();
	
	for (std::list<Node *>::iterator it=frontier_top->neighborhood.begin(); it != frontier_top->neighborhood.end(); ++it){
	    new_weigh = frontier_top->weight_so_far + (*it)->resolution + + goal->heuristique( (*it) );
	    
	    if(new_weigh < (*it)->weight_so_far){
		(*it)->weight_so_far = new_weigh;
		(*it)->coming_from = frontier_top;
		frontier.push(*it);
	    }
	}
    }
    
    // get the path recursively :
    
    if (!frontier.empty() && frontier.top()->isGoal() ){
	temp = frontier.top();
	frontier.pop();
	while ( temp != NULL){
	    (*path).push_front(temp);
	    temp = temp->coming_from;
	}
    }
}

int main(){

    std::array<Node*, 4> sons = {NULL,NULL,NULL,NULL};
    std::list<Node *> neighborhood1; 
    std::list<Node *> neighborhood2;

    Node n1(0, 0, 4, NULL, FREE_NODE, sons, neighborhood1);
    Node n2(4, 0, 4, NULL, FREE_NODE, sons, neighborhood1);
    Node n3(8, 0, 4, NULL, GOAL_NODE, sons , neighborhood2);


    neighborhood1.push_front(&n2);
    neighborhood2.push_front(&n1);
    neighborhood2.push_front(&n3);
    
    n1.neighborhood = neighborhood1;
    n2.neighborhood = neighborhood2;
    
    std::list<Node *> path;
    
    
    a_star_qt(&n2, &n3, &path);
    int res;
    
    if ( !(path.empty()) && path.back()->isGoal() ){
	std::cout << "Path is okay :" << std::endl;
	for (std::list<Node *>::iterator it = path.begin(); it != path.end(); it++){
		res = (*it)->resolution/2;
	    std::cout << (*it)->x+res<< ' ' << (*it)->y+res << std::endl;
	}
	
    }
    else{
	std::cerr << "Path is not okay" << std::endl;
	exit(-1);
    }

  
    return 0;
}
		      
#endif
