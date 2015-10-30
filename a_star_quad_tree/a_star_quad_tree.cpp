#include "node.hpp"

#ifndef A_STAR_QT
#define A_STAR_QT

std::List<Node *> a_star_qt(Node *beginning, Node *goal){
    
    std::priority_queue<Node*, vector<Node*>, DereferenceCompareNode> frontier;
    std::List<Node *> path;
    Direction *d;
    
    exploreBeginning(beginning, frontier, path, &d);

    //use direction for cost (cost higher if changing direction and for quad trees) with Dx and D
    
    while(!frontier.isEmpty() && !(frontier.top().isGoal()) ){ /
	//while frontier not empty and goal not found

	exploreFrontier(frontier, path, &d);	
	
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

void exploreBeginning(Node *beginning, Node *goal, std::priority_queue<Node*, vector<Node*>, DereferenceCompareNode> frontier, std::list<Node *> path, Direction *d){
    //Maj du cout
    //Maj de frontier en rajoutant les voisins (changer leurs attributs)

    Direction new_d;
    
    beginning->weight_so_far=beginning->resolution;
    
    for (std::list<Node *>::iterator it=beginning->neighborhood.begin(); it != beginning->neighborhood.end(); ++it){
	new_d = new_direction( (*it), frontier_top);

	(*it)->weight_so_far=beginning->weight_so_far + (*it)->resolution + goal.heuristique( (*it) );
	(*it)->coming_from = beginning;
	(*it)->previous_d = new_d;
	
	frontier.push(*it);
    }
}

void exploreFrontier(Node *beginning, Node *goal, std::priority_queue<Node*, vector<Node*>, DereferenceCompareNode> frontier, std::list<Node *> path){
    //Maj du cout (cost so far + heuristique + malus si changement de d)
    //Maj du frontier + pop top si poids plus petit

    Node * frontier_top = frontier.pop();
    int new_weigh;
    Direction new_d;
    
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
