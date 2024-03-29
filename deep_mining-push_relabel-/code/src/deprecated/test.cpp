#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cassert>
#include "../include/test.h"


// PushRelabel Function Tests - TBD

// Edge Testes - TBD

// Node Priority List Test - TBD


// generate new graph, feed it to the solver and then my solver and assert?
    // use a bash for it... 

bool testNodeUpdates(PushRelabel& graph_in_solver) {
    std::ifstream input_graph("graphs/updates.txt");
    unsigned m, n, s, t;
    int node, new_height, new_excess;
    bool keep_updating = true;

    read_graph(input_graph, n, m, s, t, graph_in_solver);
    while(keep_updating) {
        std::cout << "\n\n\nInform the node to be changed (start in 1). Input out of range to stop, 0 < i < n+1.\n";
	std::cin >> node;
	if(node > 0 and node < n+1) {
	    std::cout << "Input the new values of height and excess flow (input negative to keep the same):\n";
	    std::cin >> new_height;
	    std::cin >> new_excess;
	    Node new_node;
	    Node old_node = graph_in_solver.getGraphNode(node-1);
	    
	    if(new_height >= 0)
	        new_node.h_ = new_height;
	    else {
	        new_node.h_ = old_node.h_;
	        new_height = old_node.h_;
	    }

	    if(new_excess >= 0)
		new_node.excessFlow_ = new_excess;
	    else {
                new_node.excessFlow_ = old_node.excessFlow_;
		new_excess = old_node.excessFlow_;
	    }

	    // testa se vector = vector, muda e tals...
	    // tem que ver se o setNode que recebe node = node ja eh o suficiente para dados do vector

	    graph_in_solver.setGraphNode(new_node, node-1);
	    Node updated = graph_in_solver.getGraphNode(node-1);

            std::cout << "Updated height: " << updated.h_ << " == " << new_height << "\n";
	    assert(updated.h_ == new_height);

	    std::cout << "Updated excess: " << updated.excessFlow_ << " == " << new_excess << "\n";
	    assert(updated.excessFlow_ == new_excess);
	}
	else
	    keep_updating = false;
    }

    Node node_n = graph_in_solver.getGraphNode(node-1);
    
    std::cout << "Node Active: false\n";
    assert(node_n.active_ == false);

    node_n.active_ = true;
    graph_in_solver.setGraphNode(node_n, node-1);
    Node active_n = graph_in_solver.getGraphNode(node-1);
    
    std::cout << "Node Active: true\n";
    assert(active_n.active_ == true);

    return true;
    // faz o mesmo com edge (pergunta u e depois v), u out of range sai, u->v q nao existe soh diz -> outra funcao?
}

bool testEdgesData(PushRelabel& graph_in_solver) { // WARNING! This funcion will use adjacency matrix, have this in mind while changing the graph
    std::ifstream tested_graph("graphs/edges.txt");
    std::ifstream input_graph("graphs/edges.txt");

    unsigned m, n, s, t, tested_n, tested_m;
    unsigned u, v, w;
    int count = 0;
    std::string line = "", dummy;
    std::stringstream linestr;

    read_graph(input_graph, n, m, s, t, graph_in_solver);
    
    if(tested_graph.is_open()) {
        while(count < 1) {
           getline(tested_graph, line);
           if(line.substr(0,5) == "p max") {
               linestr.str(line);
               linestr >> dummy >> dummy >> tested_n >> tested_m;
               count++;
           }
        }
        int adj_matrix[n][n];
	for(int i=0;i<n;i++) {
	    for(int j=0;j<n;j++)
                adj_matrix[i][j] = -1;
	}
        
	while(getline(tested_graph, line)) {
            if(line.substr(0,2) == "a ") {
                std::stringstream arc(line);
                arc >> dummy >> u >> v >> w;
                adj_matrix[u-1][v-1] = w;
            }
        }
	
	std::cout << "\nSubtract -1 from U and V for the node numbers inside the Data Structures\n\n";
	for(int i=0;i<n;i++) {
	    int num_out_edges = 0;
	    std::pair<Node, std::vector<Edge>> adj_list;
	    adj_list = graph_in_solver.getAdjacencyList(i);
	    for(int j=0;j<n;j++) {
	        if(adj_matrix[i][j] != -1) {
		    num_out_edges++;
		    bool found_v_or_end = false;
		    int k = 0;
		    while(!found_v_or_end) {
		        if(adj_list.second[k].destNode_ == j) {
			    std::cout << i+1 << " ----> " << j+1 << "\n";
			    std::cout << "Capacity: " << adj_list.second[k].capacity_ << " == " << adj_matrix[i][j] << "\n";
			    assert(adj_matrix[i][j] == adj_list.second[k].capacity_);
			    found_v_or_end = true;
			}
			if(k == adj_list.second.size()) {
			    found_v_or_end = true;
			}
			k++;
		    }
		}
	    }
	    std::cout << "Num of edges: " << adj_list.second.size() << " == " << num_out_edges << "\n";
	    assert(adj_list.second.size() == num_out_edges);
	}
    }
    else
        return false;
    return true;
}

bool testGraphData(PushRelabel& graph_in_solver) { // this funcion will read an example graph file (not input)

    std::ifstream tested_graph("graphs/graphData.txt");
    std::ifstream input_graph("graphs/graphData.txt");
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

	std::cout << "biggest: " << biggest << "(" << biggest+1 << ")" << " == " << tested_n-1 << "\n";
	assert(biggest == tested_n-1);

	std::cout << "smallest: " << smallest << "(" << smallest+1 << ")" << " == 0\n";
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
    else
        return false;

    return true;
}
