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
    bool push(int u); 


public:
    
    // Setters and Getters
    void setS(unsigned);
    inline int getS() {return s_;}
    void setT(unsigned);
    inline int getT() {return t_;}

    // Main Max Flow S-T problem functions
    void updateFlow(int u, int v, int new_f);
    void updateReverseEdgeFlow(int i, int flow);
    void getMaxFlow(int s, int t);
};
