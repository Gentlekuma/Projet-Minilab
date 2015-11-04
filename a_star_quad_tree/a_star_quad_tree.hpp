#ifndef A_STAR_QT_HPP
#define A_STAR_QT_HPP

#include <queue>
#include <vector>
#include "a_star_quad_tree.hpp"
#include "node.hpp"

std::list<Node *> a_star_qt(Node *beginning, Node *goal);

int malus(Direction new_d, Direction d);

Direction new_direction(Node *from, Node *to);
		      
#endif
