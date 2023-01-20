#include "graph.h"
#include <queue>

struct Operations {
    int pushes_ = 0;
    int relabels_ = 0;
};


class PushRelabel : public Graph {

private: 
    // Start and Target
    int s_, t_;
 
    // Priotity List
    std::queue<int> priority_;

    // Pre-Flow Initialization
    void preFlow(int s);

    // Basic and core Algorithm operations
    void relabel(int u);
    bool push(int u, int v, int idx_v); 


public:
    
    // Setters and Getters
    void setS(unsigned);
    inline int getS() {return s_;}
    void setT(unsigned);
    inline int getT() {return t_;}
     
    // Push and Relabel requirements
    bool uIsActive(Node);
    bool vIsRightBelow(Node, Node);

    // Main Max Flow S-T problem function
    int getMaxFlow(int s, int t, Operations&);
    void buildAlreadyBelow(int u);
    void mergeOutEdges(int u);
};
