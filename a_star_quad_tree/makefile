EXEC = test_a_star_quad_tree
FLAGS = -Wall -std=c++11

all: $(EXEC)

test_a_star_quad_tree: a_star_quad_tree.o
	g++ $(FLAGS) -g -o test_a_star_quad_tree a_star_quad_tree.o 

a_star_quad_tree.o: a_star_quad_tree.cpp node.hpp a_star_quad_tree.hpp
	g++ $(FLAGS) -c -g a_star_quad_tree.cpp 

clean:
	rm -f *o *~ $(EXEC)
