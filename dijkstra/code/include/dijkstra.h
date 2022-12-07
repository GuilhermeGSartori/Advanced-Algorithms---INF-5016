#define INF 0x3f3f3f3f
#include <vector>
#include <unordered_map>

struct VisitedFlag {
	bool visited = false;
};

struct Edge {
	int dest_node;
	int distance;
	Edge(const int dest_node, const int distance) : dest_node(dest_node), distance(distance) {}
};

struct Operations {
	int inserts = 0;
	int updates = 0;
	int deletes = 0;
};


class Graph {

private:
	int numberOfNodes_;
	std::vector<std::vector<Edge>> graph_;

public:
	
	Graph() : numberOfNodes_(0) {}
	std::vector<Edge> getNodeList(int node);
	inline void setNumberOfNodes(const int& n) {numberOfNodes_ = n;}
	inline const int getNumberOfNodes() const {return numberOfNodes_;}
	inline void allocateVector() {graph_.resize(numberOfNodes_, std::vector<Edge>());}
	
	void addEdge(int u, int v, int w);
};



class Dijkstra {

private:
	std::unordered_map<int, VisitedFlag> visitedNodes_;

public:
	Dijkstra();
	int run(Graph&, int, int, int, Operations&);
};
