#ifndef A_STAR_QT_HPP
#define A_STAR_QT_HPP

#include <queue>
#include <vector>
#include "a_star_quad_tree.hpp"
#include "node.hpp"

void a_star_qt(Node *beginning, Node *goal, std::list<Node *> * path);
		      
#endif
