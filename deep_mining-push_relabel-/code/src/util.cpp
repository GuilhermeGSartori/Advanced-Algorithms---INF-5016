#include <sstream>
#include <iostream>
#include "../include/util.h"

#define INF 10000000

void read_graph(std::istream& in, unsigned& n, unsigned& m, unsigned& s, unsigned& t, PushRelabel& graph_in_solver) {
	
    std::string line="", dummy;
    unsigned w, h;

    getline(in,line);
    std::stringstream s_line(line);
    s_line >> w >> h;
    graph_in_solver.setH(h);
    graph_in_solver.setW(w);

    //std::cout << "w: " << w << "\n";
    //std::cout << "h: " << h << "\n";

    graph_in_solver.setNumberOfNodes(w*h+2); // every cell is a node, and a s and t node are created too
    //std::cout << "n: " << w*h+2 << "\n";
    //std::cout << "last: " << w*h+2-1 << "\n";
    graph_in_solver.allocateVectors();
    graph_in_solver.setS(0);
    s = 0;
    graph_in_solver.setT(w*h+2-1);
    t = w*h+2-1;

    unsigned i=0;
    //unsigned cell_value;
    int cell_value;
    unsigned cell_n = 1;

    while(i<h) {
	getline(in,line);
	std::stringstream full_line(line);
         
	for(int j=0;j<w;j++) {
	    full_line >> cell_value; 
	    //std::cout << "value haha:\n";
	    //std::cout << cell_value << "\n";
	    //std::cout << cell_n << "\n";
	    if(i > 0) {
		//std::cout << "cell n: " << cell_n << "\n";
		//std::cout << cell_n - w << "\n";
		graph_in_solver.addEdge(cell_n, cell_n-w, INF);
	        if(j == 0) {
	            graph_in_solver.addEdge(cell_n, cell_n-w+1, INF);
		}
	        else if(j == w-1)
	            graph_in_solver.addEdge(cell_n, cell_n-w-1, INF);
	        else {
	            graph_in_solver.addEdge(cell_n, cell_n-w-1, INF);
	            graph_in_solver.addEdge(cell_n, cell_n-w+1, INF);
	        }
	    }
	    
	    if(cell_value > 0) {
		//std::cout << "this cell value will be related to s: " << cell_n << " " << cell_value << "\n";
	        graph_in_solver.addEdge(0, cell_n, cell_value);
	    }
	    
	    else if(cell_value < 0) {
		//std::cout << "value: " << -1*cell_value << "\n";
		graph_in_solver.addEdge(cell_n, w*h+2-1, -1*cell_value);
	    }
	    
	    cell_n++;
	}

	i++;
    
    }
    //std::cout << "its the boys\n";
}
