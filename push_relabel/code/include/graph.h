#include <vectors>

/*Start of problem related structures:
  - Change internal content to adapt the graph to a specific problem.
*/
struct Edge {
    int destNode_;
    int currentFlow_;
    int capacity_;

    Edge(const int dest, const int c) : destNode_(dest), currentFlow_(0), capacity_(c) {}
};

struct Node {
    int h;
    int excessFlow_;
};
/*End of problem related structures*/



/*Generic Graph Class*/
class Graph {

private:
    // Graph used to read the input file and residual graph (data manipulation) will be the same
    // Graph size = n(Nodes) + m(Edges) -> entender melhor isso.. Default pair, vetor, referencia, vetor vazio, alocar
    int numberOfNodes_;
    std::vector<std:::pair<Node, std::vector<Edge>> graph_;
    

public:
    // Constructor and allocation
    Graph() : numberOfNodes_(0) {}
    inline void allocateVectors() {graph_.resize(numberOfNodes_, std::Pair<Node, std::vector<Edge>());}

    // Setters and Getters
    inline void setNumberOfNodes(int n) {numberOfNodes_ = n;}
    inline int getNumberOfNodes() {return numberOfNodes_;}
    
    std::pair<Node, std::vector<Edge>> getAdjacencyList(int node);

    bool setGraphNode(Node updated, int node_number);
    Node getGraphNode(int node_number);

    bool setGraphEdge(int u, int v, Edge updated);
    Edge getGraphEdge(int u, int v);

    // U --capacity W--> V
    bool addEdge(int u, int v, int w); //testa se nodo que vai inserir existe e tals, essas coisas

};

/* Graph Example:
* 0 ---> 1
*   \    ^
*    \   | 
*     -> 2
* ---------------
* Vector
* [0(u)] -> Pair:
*                Node(-, -)
*                Vector
*                  [0] -> Edge(1(v), -, -)
*                  [1] -> Edge(2(v), -, -)
* [1(u)] -> Pair:
*                Node(-, -)
*                Vector
*                  null
* [2(u)] -> Pair:
*                Node(-, -)
*                Vector
*                 [0] -> Edge(1(v), -, -)
*
* End of Graph Example */




