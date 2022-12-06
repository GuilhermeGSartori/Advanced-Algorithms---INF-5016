#include <vector>
#include <unordered_map>

enum HeapfyStatus {
	IsRoot,
	IsBalanced,
	IsLeaf,
	BecameLeaf,
};

struct node {
	int node_number, shortest_dist;
};

class KHeap {
	
private:

	std::unordered_map<int, int> nodeToHeapIdx_;
	std::vector<node> priorityQueue_;
	int K_;


public:

	//constructor
	KHeap(int k) : K_(k) {};
	
	//functions to consume and check data from the heap
	node deleteMin();
	int parentIdx(int);		
	int sonIdx(int, int);
	void printHeap();
	inline int heapSize() const { return static_cast<int>(priorityQueue_.size()); }
	inline bool isEmpty() const { return priorityQueue_.empty(); }

	//functions to update the heap
	bool addNode(node);
	bool updateDistance(int, int);
	void swap(int, int);
	HeapfyStatus heapfyUp(int);
	HeapfyStatus heapfyDown(int);
};
