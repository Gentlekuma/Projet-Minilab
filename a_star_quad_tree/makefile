EXEC = test_a_star_qt
FLAGS = -Wall

all: $(EXEC)

test_a_star_qt: a_star_qt.o
	g++ $(FLAGS) -g -o test_a_star_qt a_star_qt.o 

a_star_qt.o: a_star_quad_tree.cpp node.hpp
	g++ $(FLAGS) -c -g a_star_quad_tree.cpp 

clean:
	rm -f *o *~ $(EXEC)
