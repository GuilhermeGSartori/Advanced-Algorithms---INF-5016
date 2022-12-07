#include "../include/dijkstra.h"
#include "../include/kHeap.h"
#include <sstream>
#include <iostream>


void read_dimacs(std::istream& in, unsigned& n, unsigned& m, Graph& problemGraph) {
	
	std::string line="", dummy;
	while(line.substr(0,4) != "p sp")
		getline(in,line);

	std::stringstream linestr;
	linestr.str(line);
	linestr >> dummy >> dummy >> n >> m;
	problemGraph.setNumberOfNodes(n);
	problemGraph.allocateVector();
	unsigned i=0;
	while(i<m) {
		getline(in,line);
		if(line.substr(0,2) == "a ") {
			std::stringstream arc(line);
			unsigned u,v,w;
			char ac;
			arc >> ac >> u >> v >> w;
			problemGraph.addEdge(u-1, v-1, w);
			i++;
		}
	}
}

void help() {

}

int main(int argc, char** argv) {
	if(argc < 3) {
		help();
		return 0;
	}

	int init = atoi(argv[1]), dest = atoi(argv[2]);
	int cost;
	int k = 2; // kHeap value
	unsigned n, m;
	Operations dijkstraOps;
	Graph problemGraph;
	Dijkstra solver;
	
	read_dimacs(std::cin, n, m, problemGraph);
	cost = solver.run(problemGraph, init-1, dest-1, k, dijkstraOps);

	if(cost == INF)
		std::cout << "inf\n";
	else
		std::cout << cost << "\n";
	
	return 0;
}

