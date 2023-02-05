#include <iostream>
#include "../include/push_relabel.h"


bool PushRelabel::uIsActive(Node u) {
    return (u.excessFlow_ > 0) ? true : false;
}


bool PushRelabel::vIsRightBelow(Node u, Node v) {
    return (u.h_ - v.h_ == 1) ? true : false;  
}


bool PushRelabel::push(int u, int v, int uv_idx) { // -> O(1)
    Edge uv = PushRelabel::getGraphEdge(u, v, uv_idx);

    // Can push to the limit of the edge OR all the excess in U
    int uv_current_cap = uv.capacity_ - uv.currentFlow_;
    int extra_flow = std::min(uv_current_cap, PushRelabel::graph_[u].first.excessFlow_);

    // flow leaves U and V is removed from the push contenders
    PushRelabel::graph_[u].first.excessFlow_ -= extra_flow;
    PushRelabel::graph_[u].first.priority_.pop_back();
    
    if(v == 0) { // v is s
        int line = (u-1)/PushRelabel::w_;
	int first_collumn = line*w_;
	int collumn = (u-1)-first_collumn;
	std::pair<int, int> pos(line, collumn);
	capturedCells_.push_back(pos);
    }
        
    // flow occupies edge UV
    uv.currentFlow_ = uv.currentFlow_ + extra_flow;
    PushRelabel::setGraphEdge(u, v, uv, uv_idx);

    // flow enters V and it is now active (if it wasnt already)
    PushRelabel::graph_[v].first.excessFlow_ += extra_flow;
    if(!PushRelabel::graph_[v].first.active_) { 
	PushRelabel::graph_[v].first.active_ = true;
	PushRelabel::priority_.push(v);
    }
        
    // Creates reverse edge V->U in the residual graph to enable flow return
    int vu_pos = PushRelabel::graph_[v].first.edgesMap_[u];
    if(vu_pos > 0) {
        Edge vu = PushRelabel::getGraphEdge(v, u, vu_pos);
	vu.currentFlow_ -= extra_flow;
	PushRelabel::setGraphEdge(v, u, vu, vu_pos);
    }
    else {
        PushRelabel::addEdge(v, u, extra_flow);
    }

    return true;
}


void PushRelabel::relabel(int u) { // -> O(2n(n-2))... O(2M)... 
    const int INF = 100000000;
    int min_h = INF;
    
    // Linear loop to find the "v" nodes with the smallest height but taller than u (future push contenders)
    for(int i=0;i<graph_[u].second.size();i++) {
	int neighbor = graph_[u].second[i].destNode_;
	int uv_current_c = graph_[u].second[i].capacity_ - graph_[u].second[i].currentFlow_;

	std::pair<int,int> contender(neighbor, i);
	if(graph_[neighbor].first.h_ < min_h and graph_[neighbor].first.h_ >= graph_[u].first.h_ and uv_current_c > 0) {
	    min_h = graph_[neighbor].first.h_;
	    graph_[u].first.priority_.clear();
	    graph_[u].first.priority_.push_back(contender);
	}
	else if(graph_[neighbor].first.h_ == min_h and uv_current_c > 0) {
	    graph_[u].first.priority_.push_back(contender);
	}
    }
    if(min_h < INF) {
        graph_[u].first.h_ = min_h+1;
    }
    else
	std::cout << "This should not be possible\n";
}


void PushRelabel::buildAlreadyBelow(int u) {

    graph_[u].first.priority_.clear();

    for(int i=0;i<graph_[u].second.size();i++) {
	int neighbor = graph_[u].second[i].destNode_;
	std::pair<int,int> contender(neighbor, i);

        int uv_true_capacity = graph_[u].second[i].capacity_ - graph_[u].second[i].currentFlow_;

	if((graph_[u].first.h_-1 == graph_[neighbor].first.h_) and uv_true_capacity > 0)
	    graph_[u].first.priority_.push_back(contender);
    }

}

void PushRelabel::preFlow(int s) {

    for(int i=0;i<graph_[s].second.size();i++) {

	int v = graph_[s].second[i].destNode_;

        Edge uv = PushRelabel::getGraphEdge(s, v, i);
	uv.currentFlow_ = uv.capacity_;
	PushRelabel::setGraphEdge(s, v, uv, i);

        int vu_pos = graph_[v].first.edgesMap_[s];

	if(vu_pos > 0) {
            Edge vu = PushRelabel::getGraphEdge(v, s, vu_pos);
            vu.currentFlow_ -= uv.currentFlow_;
            PushRelabel::setGraphEdge(v, s, vu, vu_pos);
        }
        else {
            PushRelabel::addEdge(v, s, uv.currentFlow_);
        }

	graph_[v].first.excessFlow_ = uv.currentFlow_;
	graph_[v].first.active_ = true;

	PushRelabel::priority_.push(v);
    }
}


int PushRelabel::getMaxFlow(int s, int t, Operations& max_flow_ops) {

    graph_[s].first.h_ = PushRelabel::getNumberOfNodes();
    PushRelabel::preFlow(s);
    
    bool active_exists = true;
    int max_flow = 0;

    while(active_exists) {

	// Uses FIFO to find the next node (will it always find the tallest one?)
	int active_u = -1;
	
        if(!PushRelabel::priority_.empty()) {
            active_u = priority_.front();
	    priority_.pop();
	    bool s_or_t = (active_u == s or active_u == t);
	    if((s_or_t) and !PushRelabel::priority_.empty()) {
	        active_u = priority_.front();
		priority_.pop();
	    }
	    else if(s_or_t) {
	        active_u = -1;
	    }
	}
       
	if(active_u != -1) {
	    // Linear search O(n-1) (all nodes related to U out edges) to find the Vs that are exactly below U (h-h == 1)
            PushRelabel::buildAlreadyBelow(active_u);

            // O(n) -> if U points to every other node and needs to push it to everyone
            while(graph_[active_u].first.excessFlow_ > 0) {
		// There are V nodes (they are on the priority list) that can receive a push (UV has capacity and V is right below)
		if(!graph_[active_u].first.priority_.empty()) {
		    int sz = graph_[active_u].first.priority_.size();
		    int v = graph_[active_u].first.priority_[sz-1].first;
		    int v_idx = graph_[active_u].first.priority_[sz-1].second;
		    
		    // O(1)
		    PushRelabel::push(active_u, v, v_idx);
		    max_flow_ops.pushes_++;
	        }

	        else {
		    // O(n)
		    PushRelabel::relabel(active_u);
		    max_flow_ops.relabels_++;
		}
	    }
	    graph_[active_u].first.active_ = false;
	}
	else
            active_exists = false;
    }
    
    Node f_s = PushRelabel::getGraphNode(s);
    max_flow = f_s.excessFlow_;
    return max_flow;
}


void PushRelabel::setS(unsigned s) { PushRelabel::s_ = s; }


void PushRelabel::setT(unsigned t) { PushRelabel::t_ = t; }
