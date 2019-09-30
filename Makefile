all: Mttp

Mttp: mttp.cpp
	g++ mttp.cpp -O3 -fopenmp 