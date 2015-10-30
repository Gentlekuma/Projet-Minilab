#include "node.hpp"

#ifndef A_STAR_QT
#define A_STAR_QT

enum Direction {DIR_N, DIR_W, DIR_S, DIR_E};

std::List<Node *> a_star_qt(Node *beginning){
    
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

void exploreBeginning(Node *beginning, std::priority_queue<Node*, vector<Node*>, DereferenceCompareNode> frontier, std::list<Node *> path, Direction *d){
    //Maj du cout (cost so far + heuristique + malus si changement de d)
    //Maj de frontier en rajoutant les voisins (changer leurs attributs)
}

void exploreFrontier(Node *beginning, std::priority_queue<Node*, vector<Node*>, DereferenceCompareNode> frontier, std::list<Node *> path, Direction *d){
    //Maj du cout
    //Maj du frontier + pop top
}

#endif
