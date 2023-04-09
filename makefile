all: clean build

build: 
	g++-12 -std=c++20 brute.cpp -o b.out
	g++-12 -std=c++20 heuristic.cpp h.out

clean: 
	$(RM) *.o *.gch a.out