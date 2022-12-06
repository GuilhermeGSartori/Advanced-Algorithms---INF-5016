#include "../include/binHeap.h"
#include <string>
#include <iostream>
#include <fstream>

/*This will be the main function to test the heap. This file will be compiled with the binHeap.h and WITHOUT the dijkstra.cpp file and only the heap will be tested*/
int main() {
	BinHeap testedHeap;
	std::vector<node> readNodes;
	std::vector<node> expectedPostOp;
	std::string line;
	node new_node;
	int num_of_nodes = 11, to_update = 7, to_remove_min = 10;
	int dummy;

	std::ifstream ExpectedHeap("files/bin_expected_heap_post_ops.txt");
	std::ifstream InputNodes("files/bin_input_nodes_heap.txt");

	while(getline(ExpectedHeap, line)) {
		new_node.node_number = std::stoi(line);
		getline(ExpectedHeap, line);
		new_node.shortest_dist = std::stoi(line);
		getline(ExpectedHeap, line);
		expectedPostOp.push_back(new_node);
	}
	while(getline(InputNodes, line)) {
		new_node.node_number = std::stoi(line);
		// std::cout << new_node.node_number << "\n";
		getline(InputNodes, line);
		new_node.shortest_dist = std::stoi(line);
		// std::cout << new_node.shortest_dist << "\n";
		getline(InputNodes, line);
		readNodes.push_back(new_node);
	}

	for(int i = 0; i < num_of_nodes; i++) {
		std::cout << "------------------------------------------------\n";
		//std::cout << "staring heap operations:\n" << i << "\n\n";
		if(i == to_update) {
			std::cout << "Will update the node: " << readNodes[4].node_number << " dist: " << readNodes[4].shortest_dist << " with value 2\n\n";
			testedHeap.updateDistance(readNodes[4], 2); // node number five
			testedHeap.printHeap();
			std::cout << "Will add node\n\n";
			testedHeap.addNode(readNodes[i]);
			// print do status atual
		}
		else if(i == to_remove_min) {
			std::cout << "Will deleteMin\n\n";
			node removed = testedHeap.deleteMin();
			// assert do deletado
		}
		else {
			std::cout << "Will add node\n\n";
			// std::cout << i << " will add node: \n";
			// std::cout << readNodes[i].node_number << "\n";
			// std::cout << readNodes[i].shortest_dist << "\n";
			testedHeap.addNode(readNodes[i]);
			// print status atual
		}
		
		testedHeap.printHeap();
		std::cout << "insert any key and press enter to continue\n";
		//std::cin >> dummy;
		std::cin >> dummy;
	}

	// testedHeap.printHeap();

	// assert resultado final (heap com expected)
}
