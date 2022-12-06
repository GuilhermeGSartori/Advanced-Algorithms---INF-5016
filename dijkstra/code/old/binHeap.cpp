#include "../include/binHeap.h"
#include <vector>
#include <iostream>

BinHeap::BinHeap() { }

node BinHeap::deleteMin() {
	node minNode; 
	int queueSize = BinHeap::heapSize();
	minNode.node_number = -1;
	minNode.shortest_dist = -1;

	if(queueSize == 0)
		return minNode;
	else if(queueSize == 1) {
		minNode = BinHeap::priorityQueue_[0];
		BinHeap::priorityQueue_.pop_back();
		return minNode;
	}
	else {
		minNode = priorityQueue_[0];
		BinHeap::priorityQueue_[0] = BinHeap::priorityQueue_[queueSize - 1];
		BinHeap::priorityQueue_.pop_back();
		BinHeap::heapfyDown(0);
		return minNode;
	}
}

int BinHeap::parentIdx(int son_idx) {
	int parent_idx = ((son_idx - 1)/2);
	int returned_idx = std::max(parent_idx, 0); 
	return returned_idx;
}

int BinHeap::leftSonIdx(int parent_idx) {
	int son_idx = parent_idx*2+1;
	int returned_idx = std::min(son_idx, BinHeap::heapSize());
	return returned_idx;
}

int BinHeap::rightSonIdx(int parent_idx) {
	int son_idx = parent_idx*2+2;
	int returned_idx = std::min(son_idx, BinHeap::heapSize());
	return returned_idx;
}

bool BinHeap::addNode(node new_node) {

	BinHeap::priorityQueue_.push_back(new_node);
	BinHeap::nodeToHeapIdx_[new_node.node_number] = BinHeap::heapSize() - 1;
	BinHeap::heapfyUp(BinHeap::heapSize() - 1);
}

HeapfyStatus BinHeap::heapfyUp(int index) {
	if(index == 0)
		return HeapfyStatus::IsRoot;
	
	int parent_idx = BinHeap::parentIdx(index);
	if(BinHeap::priorityQueue_[parent_idx].shortest_dist > BinHeap::priorityQueue_[index].shortest_dist) {
		BinHeap::swap(parent_idx, index);
		BinHeap::heapfyUp(parent_idx);
	}

	return HeapfyStatus::IsBalanced;
}

HeapfyStatus BinHeap::heapfyDown(int index) {
	int leftSon_idx = BinHeap::leftSonIdx(index);
	bool rightIsSmaller = false, leftIsSmaller = false;

	if(leftSon_idx == BinHeap::priorityQueue_.size())
		return HeapfyStatus::IsLeaf;

	int rightSon_idx = BinHeap::rightSonIdx(index);
	leftIsSmaller = (BinHeap::priorityQueue_[leftSon_idx].shortest_dist < BinHeap::priorityQueue_[index].shortest_dist);

	if(rightSon_idx == BinHeap::heapSize()) {
		if (leftIsSmaller)
			BinHeap::swap(leftSon_idx, index);
		return HeapfyStatus::BecameLeaf; 
	}
	// tem 2 filhos
	else {
		rightIsSmaller = (BinHeap::priorityQueue_[rightSon_idx].shortest_dist < BinHeap::priorityQueue_[index].shortest_dist);
		if (leftIsSmaller or rightIsSmaller) {
			if (BinHeap::priorityQueue_[leftSon_idx].shortest_dist < BinHeap::priorityQueue_[rightSon_idx].shortest_dist) {
				BinHeap::swap(leftSon_idx, index);
				BinHeap::heapfyDown(leftSon_idx);
			}
			else {
				BinHeap::swap(rightSon_idx, index);
				BinHeap::heapfyDown(rightSon_idx);
			}
		}
		return HeapfyStatus::IsBalanced;
	}
}

void BinHeap::swap(int first_idx, int second_idx) {
	node first_node = BinHeap::priorityQueue_[first_idx];
	node second_node = BinHeap::priorityQueue_[second_idx];

	//swaps "node indexes in the heap" between key values (the actual node value) in the map
	std::swap(BinHeap::nodeToHeapIdx_[first_node.node_number], BinHeap::nodeToHeapIdx_[second_node.node_number]);
	//swaps "node types" between positions of the vector
	std::swap(BinHeap::priorityQueue_[first_idx], BinHeap::priorityQueue_[second_idx]);
}

bool BinHeap::updateDistance(node updated_node, int new_value) {
	int node_in_heap = BinHeap::nodeToHeapIdx_[updated_node.node_number];
	int old_value = BinHeap::priorityQueue_[node_in_heap].shortest_dist;

	BinHeap::priorityQueue_[node_in_heap].shortest_dist = new_value;
	if (new_value < old_value)
		BinHeap::heapfyUp(node_in_heap);
}


void BinHeap::printHeap() {
	int nodeCounter = 0;
	for(node heapNodes : BinHeap::priorityQueue_) {
		std::cout << "Node " << nodeCounter << " in the Heap:\n";
		std::cout << "Number: " << heapNodes.node_number << "\n";
		std::cout << "Distance: " << heapNodes.shortest_dist << "\n\n";
		nodeCounter++;
	}
}
