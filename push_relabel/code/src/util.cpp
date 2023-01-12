#include <sstream>
#include <iostream>
#include "../include/util.h"


void read_graph(std::istream& in, unsigned& n, unsigned& m, unsigned& s, unsigned& t, PushRelabel& graph_in_solver) {
	
    std::string line="", dummy;
    while(line.substr(0,5) != "p max")
        getline(in,line);

    std::stringstream linestr;
    linestr.str(line);
    linestr >> dummy >> dummy >> n >> m;
    graph_in_solver.setNumberOfNodes(n);
    graph_in_solver.allocateVectors();
    unsigned i=0;

    bool found_s = false;
    bool found_t = false;
    std::cout << "now we will read!\n";
    while(!found_s || !found_t) {
	getline(in,line);
        if(line.substr(0, 2) == "n ") {
	    std::stringstream s_or_t;
	    char n, stc;
	    if(line.find("s") != std::string::npos) {
	        s_or_t >> n >> s >> stc;
		graph_in_solver.setS(s);
	        found_s = true;
	    }
	    if(line.find("t") != std::string::npos) {
		s_or_t >> n >> t >> stc;
		graph_in_solver.setT(t);
		found_t = true;
	    }
	}
    }
    while(i<m) {
	getline(in,line);
	if(line.substr(0,2) == "a ") {
	    std::stringstream arc(line);
	    unsigned u,v,w;
            char ac;
            arc >> ac >> u >> v >> w;
            graph_in_solver.addEdge(u-1, v-1, w);
	    i++;
	}
    }
}
