all: clean build

build: 
	g++-12 -std=c++20 brute.cpp -O3 -o b.out
	g++-12 -std=c++20 heuristic.cpp -O3 -o h.out

clean: 
	$(RM) *.o *.gch *.out