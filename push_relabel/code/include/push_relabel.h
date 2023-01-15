#include "graph.h"

struct Operations {};


class PushRelabel : public Graph {

private: 
    // Start and Target
    int s_, t_;
    
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
    void getMaxFlow(int s, int t);
    void buildAlreadyBelow(int u);
};
