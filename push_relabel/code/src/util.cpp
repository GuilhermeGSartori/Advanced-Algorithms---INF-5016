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
    int added_edges = 0;

    bool found_s = false;
    bool found_t = false;

    while(!found_s || !found_t) {
	getline(in,line);
        if(line.substr(0, 2) == "n ") {
	    char n, stc;
	    if(line.find("s") != std::string::npos) {
		std::stringstream s_line(line);
		s_line >> n >> s >> stc;
		s--;
		graph_in_solver.setS(s-1);
	        found_s = true;
	    }
	    if(line.find("t") != std::string::npos) {
		std::stringstream t_line(line);
		t_line >> n >> t >> stc;
		t--;
		graph_in_solver.setT(t-1);
		found_t = true;
	    }
	}
    }
    while(i<m) {
	getline(in,line);
	if(line.substr(0,2) == "a ") {
	    added_edges += 1;
	    std::stringstream arc(line);
	    unsigned u,v,w;
            char ac;
            arc >> ac >> u >> v >> w;
            graph_in_solver.addEdge(u-1, v-1, w);
	    i++;
	}
    }
    if(added_edges != m)
        std::cout << "WRONG NUMBER OF ADD EDGES!\n";
}
