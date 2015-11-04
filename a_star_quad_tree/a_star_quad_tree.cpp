#ifndef A_STAR_QT
#define A_STAR_QT

#include <queue>
#include <vector>

#include "node.hpp"

class Compare{
    public:
	bool operator()(const Node* lhs, const Node* rhs){
	    return lhs->weight_so_far > rhs->weight_so_far;
	}
};


std::list<Node *> a_star_qt(Node *beginning, Node *goal){
    std::priority_queue<Node*, vector<Node*>, Compare> frontier;
    std::List<Node *> path;
    Direction d;
    Direction new_d;
    Node * frontier_top;
    int new_weigh;

    //Explore the first node :

    beginning->weight_so_far=beginning->resolution;
    
    for (std::list<Node *>::iterator it=beginning->neighborhood.begin(); it != beginning->neighborhood.end(); ++it){
	new_d = new_direction( it, frontier_top);
	it->weight_so_far = beginning->weight_so_far + it->resolution + goal.heuristique( it );
	it->coming_from = beginning;
	it->previous_d = new_d;
	
	frontier.push(it);
    }
    
    while(!frontier.isEmpty() && !(frontier.top().isGoal()) ){ 
	//while frontier not empty and goal not found

	frontier_top = frontier.top();
	frontier.pop();
	
	for (std::list<Node *>::iterator it=frontier_top->neighborhood.begin(); it != frontier_top->neighborhood.end(); ++it){
	    new_d = new_direction( (*it), frontier_top);
	    new_weigh = frontier_top->weight_so_far + (*it)->resolution + + goal.heuristique( (*it) ) + malus(new_d, frontier_top->previous_d);
	    
	    if(new_weigh < (*it)->weight_so_far){
		(*it)->weight_so_far = new_weigh;
		(*it)->coming_from = frontier_top;
		(*it)->previous_d = new_d;
		
		frontier.push(*it);
	    }
	}
    }
    
    // get the path recursively :
    
    if (!frontier.std::top().isGoal()){
	Node temp = frontier.top();
	while ( temp != NULL){
	    path.push(temp);
	    temp = temp->coming_from;
	}
    }
    
    if (std::end(path).isGoal() ){
	std::cout << "is Okay" << std::endl;
	return path;
    }
    else{
	std::cerr << "ZE PATH IS NOT FINISHED SCHIESSE" << std::endl;
	exit(-1);
    }
}

int malus(Direction new_d, Direction d){
    //get a malus for every turn to maximise the length of straight paths starting from the first node
    if (new_d != d)
	return 1;
    else
	return 0;
}

Direction new_direction(Node *from, Node *to){
    int delta_x = to->x - from->x;
    int delta_y = to->y - from->y;
    
    if (abs(delta_x) > abs(delta_y)){
	if (delta_x >=0){
	    return DIR_E;
	}
	else{
	    return DIR_W;
	}
    }
    else{
	if (delta_y >=0){
	    return DIR_S
		}
	else{
	    return DIR_N;
	}
    }
}

		      
#endif
