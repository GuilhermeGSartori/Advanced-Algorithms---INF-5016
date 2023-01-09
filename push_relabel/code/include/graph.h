

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



/*Generic Graph class*/
class Graph {

private:
    //Graph used to read the input file and residual graph (data manipulation) will be the same
    int numberOfNodes_;
    std::vector<std:::pair<Node, std::vector<Edge>> graph_;
    

public:
    // Constructor and allocation
    Graph() : numberOfNodes_(0) {}
    bool allocateVectors();

    // Setters and Getters
    inline void setNumberOfNodes(int n) {numberOfNodes_ = n;}
    inline int getNumberOfNodes() {return numberOfNodes_;}
    
    std::pair<Node, std::vector<Edge>> getAdjacencyList(int node);

    void setGraphNode(Node updated, int node_number);
    Node getGraphNode(int node_number);

    void setGraphEdge(int u, int v, Edge updated);
    Edge getGraphEdge(int u, int v);

    //u --capacity w--> v
    bool addEdge(int u, int v, int w); //testa se nodo que vai inserir existe e tals, essas coisas

};

/* Graph example:
* 0 ---> 1
*   \    ^
*    \   | 
*     -> 2
* ---------------
* Vector
* [0] -> Pair:
*             Node(x, x)
*             Vector
*               [0] -> Edge(1, 0, 6)
*               [1] -> Edge(2, 0, 5)
* [1] -> Pair:
*             Node(x, x)
*             Vector
*               null
* [2] -> Pair:
*             Node(x, x)
*             Vector
*              [0] -> Edge(1, 0, 7)
*
* End of Graph Example */




