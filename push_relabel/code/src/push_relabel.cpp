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
        
    // flow occupies edge UV
    uv.currentFlow_ = uv.currentFlow_ + extra_flow;
    PushRelabel::setGraphEdge(u, v, uv, uv_idx);

    // flow enters V
    v_n.excessFlow_ += extra_flow;
    PushRelabel::setGraphNode(v_n, v); 
        
    std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(v);
    std::vector<Edge> adj_list = full_adj.second; 
    for(int i=0;i<adj_list.size();i++) {
	if(adj_list[i].destNode_ == u) {
	    Edge vu = PushRelabel::getGraphEdge(v, u, i);
            vu.currentFlow_ -= extra_flow;
	    PushRelabel::setGraphEdge(v, u, vu, i);
	    return true;
	}
    }
    // ISSO AVACALHA COMPLEXIDADE SIM! POIS PARA CADA PUSH SERIA PERCORRIDO O(n-1) (edges) PARA MERGEAR, SENDO QUE DA PRA FAZER TUDO DE UMA VEZ PERCORRENDO 2 VEZES OS EDGES EM VEZ DE N(pode rolar N PUSH

    // Creates reverse edge V->U in the residual graph to enable flow return
    PushRelabel::addEdge(v, u, extra_flow);
    return true;
}

/*void PushRelabel::mergeOutEdges(int u) {
    int n = PushRelabel::getNumberOfNodes();
    int merger[n];
    for(int i=0;i<n;i++)
        merger[i] = -1;

    int current_edge = 0;
    bool merged_all = false;
    int start_of_repeats = -1;
    while(!merged_all) {
	std::vector<Edge> out_edges = PushRelabel::getAdjacencyList(u).second;
        if(current_edge == out_edges.size())
	    merged_all = true;

	if(!merged_all) {
	    int v = out_edges[current_edge].destNode_;
	    if(merger[v] == -1) {
	        merger[v] = current_edge;
	        current_edge++;
	    }
	    else {
		if(start_of_repeats == -1)
		    start_of_repeats = current_edge;
	        int first_found_idx = merger[v];
	        Edge og_uv = PushRelabel::getGraphEdge(u, v, first_found_idx);
		//Edge nw_uv = PushRelabel::getGraphEdge(u, v, current_edge);
		//if(nw_uv.capacity_ == 0) {
	        og_uv.currentFlow_ -= out_edges[current_edge].capacity_;
		PushRelabel::setGraphEdge(u, v, og_uv, first_found_idx);
	        PushRelabel::removeGraphEdge(u, v, current_edge); // a ultima provavelmente vai ser reverse tambem
		//}
		else {
                    nw_uv.currentFlow_ -= out_edges[first_found_idx].capacity_;
                    PushRelabel::setGraphEdge(u, v, og_uv, current_edge);
                    PushRelabel::removeGraphEdge(u, v, first_found_idx); // a ultima provavelmente vai ser reverse tambem
		    last = current_edge;
	        } 
	       	//teoricamente soh trocaria com edges novos tambem e tals, nenhum dos novos sofreria essas coisas de remove.. ENTAO EH ISSO! DAR POP A PARTIR DO PRIMEIRO MOMENTO QUE ACHOU REPETIDO!
	    }
	}
    }
    if(start_of_repeats != -1) {
        for(int i=out_edges.size();i>start_of_repeats;i--) 
 	    
        
    }
}*/


void PushRelabel::relabel(int u) { // -> O(2n(n-2))... O(2M)... 
    const int INF = 100000000;
    int min_h = INF;
 
    std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(u);
    std::vector<Edge> adj_list = full_adj.second;
    // Linear loop to merge out edges native to the graph and out edges created by the residual graphc- NOT

    Node n_u = PushRelabel::getGraphNode(u);
    // Linear loop to find the "v" nodes with the smallest height but taller than u (future push contenders)
    for(int i=0;i<adj_list.size();i++) {
	int neighbor = adj_list[i].destNode_;
	int uv_current_c = adj_list[i].capacity_ - adj_list[i].currentFlow_;
	Node v = PushRelabel::getGraphNode(neighbor);
	std::pair<int,int> contender(neighbor, i);
	if(v.h_ < min_h and v.h_ >= n_u.h_ and uv_current_c > 0) { 
	    min_h = v.h_;
            n_u.priority_.clear();
	    n_u.priority_.push_back(contender);
	}
	else if(v.h_ == min_h and uv_current_c > 0)
	    n_u.priority_.push_back(contender);
    }
    if(min_h < INF)
        n_u.h_ = min_h+1;
    else
	std::cout << "This should not be possible\n";

    PushRelabel::setGraphNode(n_u, u);
}


void PushRelabel::buildAlreadyBelow(int u) {

    Node n_u = PushRelabel::getGraphNode(u);
    n_u.priority_.clear();

    std::vector<Edge> out_edges = PushRelabel::getAdjacencyList(u).second;

    for(int i=0;i<out_edges.size();i++) {
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
        Edge uv = PushRelabel::getGraphEdge(s, 0, i); // replace zero later
	uv.currentFlow_ = uv.capacity_;
	PushRelabel::setGraphEdge(s, 0, uv, i);
        bool has_edge = false;
	int v = uv.destNode_;
	std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(v);
        std::vector<Edge> adj_list = full_adj.second;
        for(int j=0;j<adj_list.size();j++) {
            if(adj_list[j].destNode_ == s) {
		Edge vu = PushRelabel::getGraphEdge(v, s, j);
                vu.currentFlow_ -= uv.currentFlow_;
		PushRelabel::setGraphEdge(v, s, vu, j);
		has_edge =  true;
		break;
            }
        }
        if(!has_edge)
	    PushRelabel::addEdge(out_edges[i].destNode_, s, uv.currentFlow_);

	Node n_v = PushRelabel::getGraphNode(v);
	n_v.excessFlow_ = uv.currentFlow_;
	PushRelabel::setGraphNode(n_v, v);
    }
}

// Complexity: O(n) * (O(n) + O(n-1) + O(n-1) + (O(n)*O(1)*O(n)))
int PushRelabel::getMaxFlow(int s, int t) {

    Node n_s = PushRelabel::getGraphNode(s);
    n_s.h_ = PushRelabel::getNumberOfNodes();
    PushRelabel::setGraphNode(n_s, s);
    // O(?)
    PushRelabel::preFlow(s);
    
    bool active_exists = true;
    int max_flow = 0;

    // O(n)
    while(active_exists) {

	// Linear search O(n) (search all nodes in graph) to find the tallest node with excess
        int active_u = -1;
	int tallest_h = -1;
	std::vector<std::pair<Node,std::vector<Edge>>> graph;
	graph = PushRelabel::getGraph();
	for(int i=0;i<PushRelabel::getNumberOfNodes();i++) {
	    bool isActive = (i != s and i != t) & (graph[i].first.excessFlow_ > 0);
	    bool tallestActive = isActive & (graph[i].first.h_ > tallest_h);
	    if(tallestActive) {
	        active_u = i;
		tallest_h = graph[i].first.h_;
            }    
	}
       
	if(active_u != -1) {
	    // O(n-1)
            //PushRelabel::mergeOutEdges(active_u);
	   
	    // Linear search O(n-1) (all nodes related to U out edges) to find the Vs that are exactly below U (h-h == 1)
            PushRelabel::buildAlreadyBelow(active_u);
	    Node u_n = PushRelabel::getGraphNode(active_u);

            // O(n) -> if U points to every other node and needs to push it to everyone
	    while(uIsActive(u_n)) { // Active = has excess
                 
		// PushRelabel::mergeOutEdges(active_u);    
		// There are V nodes (they are on the priority list) that can receive a push (UV has capacity and V is right below)
	        if(!u_n.priority_.empty()) { 
		    int sz = u_n.priority_.size();
		    int v = u_n.priority_[sz-1].first;
		    int v_idx = u_n.priority_[sz-1].second;
		    // O(1)
		    PushRelabel::push(active_u, v, v_idx);
	        }

	        else {
		    // O(n)
		    PushRelabel::relabel(active_u);
		}
	        u_n = PushRelabel::getGraphNode(active_u);
	    }
	}
	else
            active_exists = false;
    }
    
    Node n_t = PushRelabel::getGraphNode(t);
    max_flow = n_t.excessFlow_;
    return max_flow;
}


void PushRelabel::setS(unsigned s) { PushRelabel::s_ = s; }


void PushRelabel::setT(unsigned t) { PushRelabel::t_ = t; }
