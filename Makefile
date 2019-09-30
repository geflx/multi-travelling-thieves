all: Mttp

Mttp: leituraGrids.cpp
	g++ Mttp.cpp -O3 -fopenmp -o mttp