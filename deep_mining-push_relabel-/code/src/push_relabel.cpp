#include <iostream>
#include "../include/push_relabel.h"


bool PushRelabel::uIsActive(Node u) {
    return (u.excessFlow_ > 0) ? true : false;
}


bool PushRelabel::vIsRightBelow(Node u, Node v) {
    return (u.h_ - v.h_ == 1) ? true : false;  
}


bool PushRelabel::push(int u, int v, int uv_idx) { // -> O(1)
    Node u_n = PushRelabel::getGraphNode(u);
    Node v_n = PushRelabel::getGraphNode(v);
    Edge uv = PushRelabel::getGraphEdge(u, v, uv_idx);


    // Can push to the limit of the edge OR all the excess in U
    int uv_current_cap = uv.capacity_ - uv.currentFlow_;
    int extra_flow = std::min(uv_current_cap, u_n.excessFlow_);

    // flow leaves U and V is removed from the push contenders
    u_n.excessFlow_ -= extra_flow;
    u_n.priority_.pop_back();
    PushRelabel::setGraphNode(u_n, u);
    if(v == 0) { // v is s
	//std::cout << "i am: " << u << "\n";
        int line = (u-1)/PushRelabel::w_;
	int first_collumn = line*w_;
	int collumn = (u-1)-first_collumn;
	std::pair<int, int> pos(line, collumn);
	capturedCells_.push_back(pos);
	//std::cout << "size: " << capturedCells_.size() << "\n";
    }
        
    // flow occupies edge UV
    uv.currentFlow_ = uv.currentFlow_ + extra_flow;
    PushRelabel::setGraphEdge(u, v, uv, uv_idx);

    // flow enters V and it is now active (if it wasnt already)
    v_n.excessFlow_ += extra_flow;
    if(!v_n.active_) {
        v_n.active_ = true;
	PushRelabel::priority_.push(v);
    }
    PushRelabel::setGraphNode(v_n, v); 
        
    // Creates reverse edge V->U in the residual graph to enable flow return
    int vu_pos =  v_n.edgesMap_[u];
    if(vu_pos > 0) {
        Edge vu = PushRelabel::getGraphEdge(v, u, vu_pos);
	vu.currentFlow_ -= extra_flow;
	PushRelabel::setGraphEdge(v, u, vu, vu_pos);
    }
    else {
        PushRelabel::addEdge(v, u, extra_flow);
    }
    //PushRelabel::addEdge(v, u, extra_flow);
    return true;
}


void PushRelabel::mergeOutEdges(int u) {
    int n = PushRelabel::getNumberOfNodes();
    //std::cout << "eh o n: " << n << "\n";
    int merger[n];
    for(int i=0;i<n;i++)
        merger[i] = -1;

    int current_edge = 0;
    bool merged_all = false;
    while(!merged_all) {
	//std::cout << "merge main loop\n";
	//std::cout << "current: " << current_edge << "\n";
	std::vector<Edge> out_edges = PushRelabel::getAdjacencyList(u).second;
        if(current_edge == out_edges.size())
	    merged_all = true;

	if(!merged_all) {
	    int v = out_edges[current_edge].destNode_;
	    //std::cout << "u: " << u << "\n";
	    //std::cout << "v: " << v << "\n";
	    if(merger[v] == -1) {
	        merger[v] = current_edge;
	        current_edge++;
	    }
	    else {
	        int first_found_idx = merger[v];
	        Edge og_uv = PushRelabel::getGraphEdge(u, v, first_found_idx);
	        og_uv.currentFlow_ -= out_edges[current_edge].capacity_;
		PushRelabel::setGraphEdge(u, v, og_uv, first_found_idx);
	        PushRelabel::removeGraphEdge(u, v, current_edge); 
	    }
	}
    }
}


void PushRelabel::relabel(int u) { // -> O(2n(n-2))... O(2M)... 
    const int INF = 100000000;
    int min_h = INF;
    //bool can_equal = false; //test
 
    std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(u);
    std::vector<Edge> adj_list = full_adj.second;
    Node n_u = PushRelabel::getGraphNode(u);

    // Linear loop to find the "v" nodes with the smallest height but taller than u (future push contenders)
    for(int i=0;i<adj_list.size();i++) {
	int neighbor = adj_list[i].destNode_;
	int uv_current_c = adj_list[i].capacity_ - adj_list[i].currentFlow_;
	Node v = PushRelabel::getGraphNode(neighbor);
	std::pair<int,int> contender(neighbor, i);
	if(v.h_ < min_h and v.h_ >= n_u.h_ and uv_current_c > 0) { 
	    min_h = v.h_;
	    //std::vector<std::pair<int,int>> new_priority;
            n_u.priority_.clear(); //O(n) -> acho que aqui zoa complexidade... Mesmo assim, acho que nao ia passar de 2n... pq o clear vai ser em subsets do n... nao em seu total.. sempre avanca..
	    n_u.priority_.push_back(contender);
	    //new_priority.push_back(contender);
	    //can_equal = true;
	}
	else if(v.h_ == min_h and uv_current_c > 0) {
	    n_u.priority_.push_back(contender);
	    //new_priority.push_back(contender);
	}
    }
    if(min_h < INF)
        n_u.h_ = min_h+1;
    else
	std::cout << "This should not be possible\n";

    //n_u.priority_ = new_priority;
    PushRelabel::setGraphNode(n_u, u);
}


void PushRelabel::buildAlreadyBelow(int u) {// acho que eh isso, nao tem o q mudar

    Node n_u = PushRelabel::getGraphNode(u);
    
    // this doesnt work...
    /*//std::vector<std::pair<int,int>> old_priority_ = n_u.priority_; 
    std::vector<std::pair<int,int>> new_priority; //int -> v, int -> onde esse contender ta nas arestas (list de adj do nodo)
    for(int i=0;i<n_u.priority_.size();i++) { // passo pela lista de prioridade herdada, pego v (first), nodo dele, vejo se already below, se sim, coloco na new_priority (igual a new)
        int v = n_u.priority_[i].first;
	Node n_v = PushRelabel::getGraphNode(v);
	if(n_v.h_ == n_u.h_ - 1) {
	    //std::pair<int,int> contender = (n_u.priority_[i].first, n_u.priority_[i].second);
	    std::pair<int, int> contender = n_u.priority_[i];
	    new_priority.push_back(contender);
	}
    }
    n_u.priority_ = new_priority;*/
    
    n_u.priority_.clear(); //O(n)

    std::vector<Edge> out_edges = PushRelabel::getAdjacencyList(u).second;

    for(int i=0;i<out_edges.size();i++) {
	//std::cout << "build main loop\n";
        int neighbor = out_edges[i].destNode_;
	std::pair<int,int> contender(neighbor, i);

	Node n_v = PushRelabel::getGraphNode(neighbor);
        int uv_true_capacity = out_edges[i].capacity_ - out_edges[i].currentFlow_;

	if(vIsRightBelow(n_u, n_v) and uv_true_capacity > 0) 
	    n_u.priority_.push_back(contender);
    }

    PushRelabel::setGraphNode(n_u, u);
}

void PushRelabel::preFlow(int s) {
    std::pair<Node,std::vector<Edge>> adj_list = PushRelabel::getAdjacencyList(s);
    std::vector<Edge> out_edges = adj_list.second;
    for(int i=0;i<out_edges.size();i++) {
	//std::cout << "size: " << out_edges.size() << "\n";
	int v = out_edges[i].destNode_;
	//std::cout << "s to v: " << v << "\n";

        Edge uv = PushRelabel::getGraphEdge(s, v, i);
	uv.currentFlow_ = uv.capacity_;
	PushRelabel::setGraphEdge(s, v, uv, i);
	
	//std::cout << "s zuado: " << s << "\n";
	
	
	// NEW STUFF
	Node n_v = PushRelabel::getGraphNode(v);
	int vu_pos = n_v.edgesMap_[s];
        if(vu_pos > 0) {
            Edge vu = PushRelabel::getGraphEdge(v, s, vu_pos);
            vu.currentFlow_ -= uv.currentFlow_;
            PushRelabel::setGraphEdge(v, s, vu, vu_pos);
        }
        else {
            PushRelabel::addEdge(v, s, uv.currentFlow_);
        }
	// END
	
	
	//PushRelabel::addEdge(v, s, uv.currentFlow_);
	//Node n_v = PushRelabel::getGraphNode(v);
	
	n_v.excessFlow_ = uv.currentFlow_;
	n_v.active_ = true;
	PushRelabel::priority_.push(v);
	PushRelabel::setGraphNode(n_v, v);
    }
}

// Complexity: O(n) * (O(n) + O(n-1) + O(n-1) + (O(n)*O(1)*O(n)))
int PushRelabel::getMaxFlow(int s, int t, Operations& max_flow_ops) {

    Node n_s = PushRelabel::getGraphNode(s);
    n_s.h_ = PushRelabel::getNumberOfNodes();
    PushRelabel::setGraphNode(n_s, s);
    // O(?)
    PushRelabel::preFlow(s);
    
    bool active_exists = true;
    int max_flow = 0;

    // O(n)
    //std::cout << "will start main loop\n";
    while(active_exists) {

	// Uses FIFO to find the next node (will it always find the tallest one?)
        // Why?
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
       
	//std::cout << "main if\n";
	if(active_u != -1) {
	    // O(n-1)
            //PushRelabel::mergeOutEdges(active_u);
	    //std::cout << "after merge\n";
	    // Linear search O(n-1) (all nodes related to U out edges) to find the Vs that are exactly below U (h-h == 1)
            PushRelabel::buildAlreadyBelow(active_u);
	    //std::cout << "after build\n";
	    Node u_n = PushRelabel::getGraphNode(active_u);
	    //std::cout << "after get\n";

            // O(n) -> if U points to every other node and needs to push it to everyone
	    while(uIsActive(u_n)) { // Active = has excess
                //std::cout << "active: " << active_u << "\n";
		//PushRelabel::mergeOutEdges(active_u);    
		// There are V nodes (they are on the priority list) that can receive a push (UV has capacity and V is right below)
	        if(!u_n.priority_.empty()) { 
		    int sz = u_n.priority_.size();
		    int v = u_n.priority_[sz-1].first;
		    int v_idx = u_n.priority_[sz-1].second;
		    // O(1)
		    PushRelabel::push(active_u, v, v_idx);
		    max_flow_ops.pushes_++;
		    std::cout << "PUSHES: " << max_flow_ops.pushes_ << "\n";
	        }

	        else {
		    // O(n)
		    PushRelabel::relabel(active_u);
		    max_flow_ops.relabels_++;
		}
	        u_n = PushRelabel::getGraphNode(active_u);
	    }
	    u_n.active_ = false;
	    PushRelabel::setGraphNode(u_n, active_u);
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
