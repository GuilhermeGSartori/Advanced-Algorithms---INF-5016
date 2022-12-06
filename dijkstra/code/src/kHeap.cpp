#include "../include/kHeap.h"
#include <vector>
#include <iostream>
#include <algorithm>


/*Data checking and consumption*/
node KHeap::deleteMin() {
	node minNode; 
	int queueSize = KHeap::heapSize();
	minNode.node_number = -1;
	minNode.shortest_dist = -1;

	if(queueSize == 0)
		return minNode;
	else if(queueSize == 1) {
		minNode = KHeap::priorityQueue_[0];
		KHeap::priorityQueue_.pop_back();
		return minNode;
	}
	else {
		minNode = priorityQueue_[0];
		KHeap::priorityQueue_[0] = KHeap::priorityQueue_[queueSize - 1];
		KHeap::priorityQueue_.pop_back();
		KHeap::heapfyDown(0);
		return minNode;
	}
}

int KHeap::parentIdx(int son_idx) {
	int parent_idx = ((son_idx - 1)/KHeap::K_);
	int returned_idx = std::max(parent_idx, 0);
	return returned_idx;
}

int KHeap::sonIdx(int parent_idx, int whichSon) {
	int son_idx = parent_idx*KHeap::K_ + whichSon;
	int returned_idx = std::min(son_idx, KHeap::heapSize());
	return returned_idx; 
}

/*Heap update functions*/
bool KHeap::addNode(node new_node) {
	KHeap::priorityQueue_.push_back(new_node);
	KHeap::nodeToHeapIdx_[new_node.node_number] = KHeap::heapSize() - 1;
	KHeap::heapfyUp(KHeap::heapSize() - 1);
}

HeapfyStatus KHeap::heapfyUp(int index) {
	if(index == 0)
		return HeapfyStatus::IsRoot;
	
	int parent_idx = KHeap::parentIdx(index);

	if(KHeap::priorityQueue_[parent_idx].shortest_dist > KHeap::priorityQueue_[index].shortest_dist) {
		KHeap::swap(parent_idx, index);
		KHeap::heapfyUp(parent_idx); //indice que agora armazena o nodo que antes era filho
	}

	return HeapfyStatus::IsBalanced;
}

HeapfyStatus KHeap::heapfyDown(int index) {
	int whichSon = 1, sonIdx;
	bool foundAllKids = false;
	int smallestIdx = index, currentSmallest = KHeap::priorityQueue_[index].shortest_dist;

	while(!foundAllKids) {
		sonIdx = KHeap::sonIdx(index, whichSon);
		if(sonIdx == KHeap::priorityQueue_.size() or whichSon == KHeap::K_+1)
			foundAllKids = true;
		else {
			if(KHeap::priorityQueue_[sonIdx].shortest_dist < currentSmallest) {
				currentSmallest = KHeap::priorityQueue_[sonIdx].shortest_dist;
				smallestIdx = sonIdx;
			}
			whichSon++;
		}
	}

	if(whichSon == 1)
		return HeapfyStatus::IsLeaf;

	else if(smallestIdx == index)
		return HeapfyStatus::IsBalanced;
	
	else if(whichSon < KHeap::K_) {
		KHeap::swap(smallestIdx, index);
		return HeapfyStatus::BecameLeaf;
	}
	
	else {
		KHeap::swap(smallestIdx, index);
		KHeap::heapfyDown(smallestIdx);
		return HeapfyStatus::IsBalanced;
	}
}

void KHeap::swap(int first_idx, int second_idx) {
	node first_node = KHeap::priorityQueue_[first_idx];
	node second_node = KHeap::priorityQueue_[second_idx];

	//swaps "node indexes in the heap" between key values (the actual node value) in the map
	std::swap(KHeap::nodeToHeapIdx_[first_node.node_number], KHeap::nodeToHeapIdx_[second_node.node_number]);
	//swaps "node types" between positions of the vector
	std::swap(KHeap::priorityQueue_[first_idx], KHeap::priorityQueue_[second_idx]);
}

bool KHeap::updateDistance(int nodeNumber, int new_value) {
	int node_in_heap = KHeap::nodeToHeapIdx_[nodeNumber];
	int old_value = KHeap::priorityQueue_[node_in_heap].shortest_dist;

	KHeap::priorityQueue_[node_in_heap].shortest_dist = new_value;
	if (new_value < old_value)
		KHeap::heapfyUp(node_in_heap);
}

void KHeap::printHeap() {
	int nodeCounter = 0;
	for(node heapNodes : KHeap::priorityQueue_) {
		std::cout << "Node " << nodeCounter << " in the Heap:\n";
		std::cout << "Number: " << heapNodes.node_number << "\n";
		std::cout << "Distance: " << heapNodes.shortest_dist << "\n\n";
		nodeCounter++;
	}
}
