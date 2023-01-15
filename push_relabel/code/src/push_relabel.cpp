#include <iostream>
#include "../include/push_relabel.h"


bool PushRelabel::uIsActive(Node u) {
    return (u.excessFlow_ > 0) ? true : false;
}


bool PushRelabel::vIsRightBelow(Node u, Node v) {
    return (u.h_ - v.h_ == 1) ? true : false;  
}


bool PushRelabel::push(int u, int v, int uv_idx) { // -> O(1)
    bool possible_push;
    Node u_n = PushRelabel::getGraphNode(u);
    Node v_n = PushRelabel::getGraphNode(v);

    if(PushRelabel::vIsRightBelow(u_n, v_n)) {
	Edge uv = PushRelabel::getGraphEdge(u, v, uv_idx); 
        Edge updated_uv(v, 0);                    
	Node new_u = u_n;
	int extra_flow = std::min(uv.capacity_, u_n.excessFlow_);

	new_u.excessFlow_ -= extra_flow;
	PushRelabel::setGraphNode(new_u, u);

	updated_uv.currentFlow_ = uv.currentFlow_ + extra_flow;
	updated_uv.capacity_ = uv.capacity_ - extra_flow;
	updated_uv.destNode_ = v;
	PushRelabel::setGraphEdge(u, v, updated_uv, uv_idx); 
        
	PushRelabel::addEdge(v, u, extra_flow);

	return true;
    }

    return false;
}


void PushRelabel::relabel(int u) { // -> O(2n(n-2))... O(2M)...
    int merger[PushRelabel::getNumberOfNodes()] = {-1}; 
    const int INF = 100000000;
    int min_h = INF;

    std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(u);
    std::vector<Edge> adj_list = full_adj.second;
    // Linear loop to merge out edges native to the graph and out edges created by the residual graph
    for(int i=0;i<adj_list.size();i++) { // use for each?
	int v = adj_list[i].destNode_;
	if(merger[v] == -1) 
	    merger[v] = i;
	else {
	    int j = merger[v];
	    Edge og_uv = PushRelabel::getGraphEdge(u, v, j);
	    og_uv.capacity_ += adj_list[i].capacity_;
            og_uv.currentFlow_ += adj_list[i].currentFlow_;
	    PushRelabel::setGraphEdge(u, v, og_uv, j);
	    PushRelabel::removeGraphEdge(u, v, i);
	}
    }

    Node n_u = PushRelabel::getGraphNode(u);
    //const int INF = 10000000;
    // Linear loop to find the "v" nodes with the smallest height (future push contenders)
    for(int i=0;i<adj_list.size();i++) {
	int neighbor = adj_list[i].destNode_;
	Node v = PushRelabel::getGraphNode(neighbor);
	std::pair<int,int> contender(neighbor, i);
	if(v.h_ < min_h and v.h_ >= n_u.h_) { 
	    min_h = v.h_;
            n_u.priority_.clear();
	    n_u.priority_.push_back(contender);
	}
	else if(v.h_ == min_h)
	    n_u.priority_.push_back(contender);
    }
    n_u.h_ = min_h+1;
    PushRelabel::setGraphNode(n_u, u);
}


void PushRelabel::buildAlreadyBelow(int u) {
    Node n_u = PushRelabel::getGraphNode(u);
    
    std::pair<Node, std::vector<Edge>> adj_list = PushRelabel::getAdjacencyList(u);
    std::vector<Edge> out_edges = adj_list.second;
 
    n_u.priority_.clear();
    for(int i=0;i<out_edges.size();i++) {
        int neighbor = out_edges[i].destNode_;
	Node v = PushRelabel::getGraphNode(neighbor);
	std::pair<int,int> contender(neighbor, i);
	if(n_u.h_ - v.h_ == 1) 
	    n_u.priority_.push_back(contender);
    }
    PushRelabel::setGraphNode(n_u, u);
}


void PushRelabel::getMaxFlow(int s, int t) {
    /*set initial variables like s, t and s height*/
    /*pre-flow*/
    while(/*there is an active node*/) {
	/*Busca linear por nodo que tem excesso com a maior altura!*/
        PushRelabel::buildAlreadyBelow(u);
	Node u_n = PushRelabel::getGraphNode(u);
        while(uIsActive(u_n)) {
	    if(!u_n.priority_.empty()) { 
	        bool push_done;
		int sz = u_n.priority_.size();
		push_done = PushRelabel::push(u, u_n.priority_[sz-1].first, u_n.priority_[sz-1].second);
	        if(push_done)
		    u_n.priority_.pop_back();
		else
	            std::cout << "There is something wrong\n";
	    }
	    else
		PushRelabel::relabel(u);
	}
    }
}


void PushRelabel::setS(unsigned s) { PushRelabel::s_ = s; }


void PushRelabel::setT(unsigned t) { PushRelabel::t_ = t; }
