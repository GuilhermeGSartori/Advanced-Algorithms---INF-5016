#include <vector>
#include <utility>

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
    int h_ = 0;
    int excessFlow_ = 0;
    std::vector<std::pair<int,int>> priority_;
};
/*End of problem related structures*/



/*Generic Graph Class*/
class Graph {

private:
    // Graph used to read the input file and residual graph (data manipulation) will be the same
    // Graph size = n(Nodes) + m(Edges) -> entender melhor isso.. Default pair, vetor, referencia, vetor vazio, alocar
    int numberOfNodes_;
    std::vector<std::pair<Node, std::vector<Edge>>> graph_;

public:
    // Constructor and allocation
    Graph() : numberOfNodes_(0) {}
    inline void allocateVectors() {graph_.resize(numberOfNodes_, std::pair<Node, std::vector<Edge>>());}

    // Setters and Getters
    inline void setNumberOfNodes(int n) {numberOfNodes_ = n;}
    inline int getNumberOfNodes() {return numberOfNodes_;}
    
    std::pair<Node, std::vector<Edge>> getAdjacencyList(int node);
    inline std::vector<std::pair<Node, std::vector<Edge>>> getGraph() {return graph_;}
    /*std::vector<Edge> &getRefToAdjList(int node);*/

    bool setGraphNode(Node updated, int node_number);
    Node getGraphNode(int node_number);

    bool setGraphEdge(int u, int v, Edge updated, int uv_idx);
    Edge getGraphEdge(int u, int v, int uv_idx);
    void removeGraphEdge(int u, int v, int uv_idx);

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
*                Node(-, -, Vector -> null)
*                Vector
*                  [0] -> Edge(1(v), -, -)
*                  [1] -> Edge(2(v), -, -)
* [1(u)] -> Pair:
*                Node(-, -, Vector -> null)
*                Vector
*                  null
* [2(u)] -> Pair:
*                Node(-, -, Vector -> null)
*                Vector
*                 [0] -> Edge(1(v), -, -)
*
*
* n + 4m -> to do: adapt with the variables
* Since the main vector is used to represent the n nodes,
*     each node will make a pair with a vector that represents his out edges
*     but during the run of the push relabel he may have 2 out edges to each neighbor (2m).
*     Each node also have a vector of the neighbors that are contenders for the push (right below),
*     since the limit is every neighbot the node has, it is 2m too.
* 
* End of Graph Example */




