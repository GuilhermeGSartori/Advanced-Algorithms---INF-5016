void readFlowGraph(std::istream& in, unsigned& n, unsigned& m, Graph& problemGraph);

struct Operations {}


class PushRelabel : public Graph {

private:
    // Start and Target
    int s, t;
    
    // Pre-Flow Initialization
    void preFlow(int s);

    // Basic and core Algorithm operations
    void relabel(int u);
    bool push(int u); 


public:
    
    // Setters and Getters
    void setS(int s);
    int getS();
    void getT(int t);
    int getT();

    // Main Max Flow S-T problem functions
    void updateFlow(int u, int v, int new_f);
    void updateReverseEdgeFlow(int i, int flow);
    void getMaxFlow(int s, int t);
};
