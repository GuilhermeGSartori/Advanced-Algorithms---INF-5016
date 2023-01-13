#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cassert>
#include "../include/test.h"


bool testGraphData(PushRelabel& graph_in_solver) { // this funcion will an example graph file

    std::ifstream tested_graph("graphs/ex.txt");
    std::ifstream input_graph("graphs/ex.txt");
    unsigned m, n, s, t, total_edges = 0;
    unsigned tested_n, tested_m;
    unsigned tested_s, tested_t;
    int count = 0, u, v, w;
    std::string line = "", dummy;
    std::stringstream linestr;

    read_graph(input_graph, n, m, s, t, graph_in_solver);
    int smallest = n+1, biggest = -1;

    if(tested_graph.is_open()) {
        while(count < 3) {
	   getline(tested_graph, line);
           if(line.substr(0,5) == "p max") {
	       linestr.str(line); // why does works ONLY here?
	       linestr >> dummy >> dummy >> tested_n >> tested_m;
	       count++;
	   }
	   else if(line.find("s") != std::string::npos) {
	       std::stringstream s_line(line);
	       s_line >> dummy >> tested_s >> dummy;
	       count++;
	   } 
	   else if(line.find("t") != std::string::npos) {
	       std::stringstream t_line(line);
	       t_line >> dummy >> tested_t >> dummy;
	       count++;
	   }

	}
	int num_out_edges[tested_n] = {0};
	int num_of_adds = 0;
	while(getline(tested_graph, line)) {
	    if(line.substr(0,2) == "a ") {
		num_of_adds += 1;
                std::stringstream arc(line);
	        arc >> dummy >> u >> v >> w;
	        num_out_edges[u-1]++;
	        if(u < smallest)
	            smallest = u - 1;
	        if(u > biggest)
	            biggest = u - 1;
	    }
	}
	
        std::vector<std::pair<Node, std::vector<Edge>>> filled_graph;
        filled_graph = graph_in_solver.getGraph();

	std::cout << "num of nodes: " << graph_in_solver.getNumberOfNodes() << " == " << tested_n << "\n";
	assert(graph_in_solver.getNumberOfNodes() == tested_n);

	std::cout << "num of added edges: " << num_of_adds << " == " << m <<  "\n";
	assert(num_of_adds == m);

	std::cout << "biggest: " << biggest << " == " << tested_n-1 << "\n";
	assert(biggest == tested_n-1);

	std::cout << "smallest: " << smallest << " == 0\n";
	assert(smallest == 0);

	std::cout << "Official S: " << graph_in_solver.getS() << " == " << tested_s << "\n";
	assert(tested_s == graph_in_solver.getS());

	std::cout << "Official T: " << graph_in_solver.getT() << " == " << tested_t << "\n";
	assert(tested_t == graph_in_solver.getT());

	std::cout << "Main vector size: " << filled_graph.size() << " == " << tested_n << "\n"; 
	assert(filled_graph.size() == tested_n);

	for(int i=0;i<n;i++) {
	    std::pair<Node, std::vector<Edge>> adj_list = graph_in_solver.getAdjacencyList(i);

	    std::cout << "Node " << i << " adj list (vec) size: " << adj_list.second.size() << " == " << num_out_edges[i] << "\n";
	    assert(adj_list.second.size() == num_out_edges[i]);
	    
	    std::cout << "Node " << i << " height: " << adj_list.first.h_ << " == 0" << "\n";
	    assert(adj_list.first.h_ == 0);

	    std::cout << "Node " << i << " excess flow: " << adj_list.first.h_ << " == 0" << "\n";
	    assert(adj_list.first.excessFlow_ == 0);
            total_edges += adj_list.second.size();
	}
	std::cout << "Sum of all nodes out edges: " << total_edges << " == " << tested_m << "\n";
	assert(total_edges == tested_m);
    }

    return true;
}
