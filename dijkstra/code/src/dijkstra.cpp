#include "../include/dijkstra.h"
//#include "../include/binHeap.h"
#include "../include/kHeap.h"
#include <vector>
#include <iostream>

Dijkstra::Dijkstra() { }

int Dijkstra::run(Graph& problemGraph, int init, int dest, int k, Operations& ops) {

	bool haveTarget = true;
	KHeap heap(k);
	std::vector<int> globalDistanceTo(problemGraph.getNumberOfNodes(), INF);
	node newHeapNode, currentNode;
	if(dest == -1) {
		dest = init;
		bool haveTarget = false;
	}	

	globalDistanceTo[init] = 0;
	newHeapNode.node_number = init;
	newHeapNode.shortest_dist = 0;
	heap.addNode(newHeapNode);
	ops.inserts++;

	while(!heap.isEmpty() and (!Dijkstra::visitedNodes_[dest].visited or !haveTarget)) {
		// nodes are used in the heap -> current node; edges are used in the graph -> the next explored nodes
		// gets a node (heap element) by deleting the min distance to a node (will be the next step of the algorithm)
		currentNode = heap.deleteMin();
		ops.deletes++;
		Dijkstra::visitedNodes_[currentNode.node_number].visited = true;

		// gets each neighbor (node that current has a edge to, an Edge, that is, a graph element) of the currentNode (min). The neighbors are found the a adjancy list.
		for(Edge edgeFromCurrent : problemGraph.getNodeList(currentNode.node_number)) {

			if(!Dijkstra::visitedNodes_[edgeFromCurrent.dest_node].visited) {
				
				// Happens if the neighbot that has an edge from the current node was found for the first time
				if(globalDistanceTo[edgeFromCurrent.dest_node] == INF) {
					globalDistanceTo[edgeFromCurrent.dest_node] = globalDistanceTo[currentNode.node_number] + edgeFromCurrent.distance;
					newHeapNode.node_number = edgeFromCurrent.dest_node; // Edge receiving end was turned in a Node and pushed into the prioroty queue
					newHeapNode.shortest_dist = globalDistanceTo[edgeFromCurrent.dest_node];
					heap.addNode(newHeapNode);
					ops.inserts++;
				}
				
				// Happens if the distance to the current node + the distance from the current node to his neighbor is smaller than the previous path available to him
				else if(globalDistanceTo[currentNode.node_number] + edgeFromCurrent.distance < globalDistanceTo[edgeFromCurrent.dest_node]) {
					globalDistanceTo[edgeFromCurrent.dest_node] = globalDistanceTo[currentNode.node_number] + edgeFromCurrent.distance;
					heap.updateDistance(edgeFromCurrent.dest_node, globalDistanceTo[edgeFromCurrent.dest_node]);
					ops.updates++;
				}
			}
		}
	}

	return globalDistanceTo[dest];
}


void Graph::addEdge(int u, int v, int w) {
	Graph::graph_[u].push_back(Edge(v, w));
}

std::vector<Edge> Graph::getNodeList(int node) {
	return Graph::graph_[node];
}
