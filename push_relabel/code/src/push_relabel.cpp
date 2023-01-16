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
	Edge uv = PushRelabel::getGraphEdge(u, v, uv_idx); // ta vindo desse cara o lixo
        Edge updated_uv = PushRelabel::getGraphEdge(u, v, uv_idx);                    
	Node new_u = u_n;
	Node new_v = v_n;
	//std::cout << "uv capacity: " << uv.capacity_ << "\n";
	//std::cout << "u_n excess flow " << u_n.excessFlow_ << "\n";
	int extra_flow = std::min(uv.capacity_-uv.currentFlow_, u_n.excessFlow_);

	//std::cout << "extra flow to be added: " << extra_flow << "\n";
	//std::cout << "current excess flow: " << new_u.excessFlow_ << "\n";
	new_u.excessFlow_ -= extra_flow;
	PushRelabel::setGraphNode(new_u, u);
        
	new_v.excessFlow_ += extra_flow;
	PushRelabel::setGraphNode(new_v, v);
        
	// int buf;
	// std::cin >> buf;

	updated_uv.currentFlow_ = uv.currentFlow_ + extra_flow;
	//updated_uv.capacity_ = uv.capacity_ - extra_flow;
	//std::cout <<  "updated uv capacity: " << updated_uv.capacity_ << "\n";
	updated_uv.destNode_ = v;
	PushRelabel::setGraphEdge(u, v, updated_uv, uv_idx); 
        
	//std::cout << "i will add this to a c: " << extra_flow << "\n";
	// aqui acho que na verdade tem que ver se v->u existe. Se sim, tira flow dele (aumenta capacidade), se nao dai cria
	// na verdade, merger pode ser isso, so tira do fluxo em vez de mergear tudo e pah
	// merger, lop infinito, lixo, resultado ficar faltando 1...
        std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(v);
        std::vector<Edge> adj_list = full_adj.second; 
	for(int i=0;i<adj_list.size();i++) {
	    if(adj_list[i].destNode_ == u) {
	        Edge vu = PushRelabel::getGraphEdge(v, u, i);
                //vu.capacity_ += extra_flow;
		vu.currentFlow_ -= extra_flow;
		PushRelabel::setGraphEdge(v, u, vu, i);
	        //adj_list[i].currentFlow_ -= extra_flow;

		return true;
	    }
	}

	PushRelabel::addEdge(v, u, extra_flow);
 
	//std::cout << "current excess: " << new_u.excessFlow_ << "\n";

	return true;
    }

    return false;
}


void PushRelabel::relabel(int u) { // -> O(2n(n-2))... O(2M)...
    int merger[PushRelabel::getNumberOfNodes()];
    for(int i=0;i<PushRelabel::getNumberOfNodes();i++)
        merger[i] = -1; 
    const int INF = 100000000;
    int min_h = INF;
 
    std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(u);
    std::vector<Edge> adj_list = full_adj.second;
    // Linear loop to merge out edges native to the graph and out edges created by the residual graph
    /*for(int i=0;i<adj_list.size();i++) { // use for each?
	int v = adj_list[i].destNode_;
	// std::cout << "v is " << v << "\n";
	// std::cout << "merger stuff: " << merger[v] << "\n";
	if(merger[v] == -1) {
	    merger[v] = i;
	    // std::cout << "to perdido man\n";
	}
	else {
            // std::cout << "se entrei aqui, deu ruim kk\n";
	    int j = merger[v];
	    Edge og_uv = PushRelabel::getGraphEdge(u, v, j);
	    //std::cout << "og capacity: " << og_uv.capacity_ << "\n";
	    //std::cout << "clone capacity: " << adj_list[i].capacity_ << "\n";
	    og_uv.capacity_ += adj_list[i].capacity_;
	    //std::cout << "capacity that was set: " << og_uv.capacity_ << "\n";
            og_uv.currentFlow_ += adj_list[i].currentFlow_;
	    PushRelabel::setGraphEdge(u, v, og_uv, j);
	    PushRelabel::removeGraphEdge(u, v, i); //tu nao pode remover de cara... fora que o ultimo ta aqui agora
	    break;
	}
	// std::cout << "o q veio antes\n";
	// std::cout << "i: " << i << "\n";
    }*/
    //std::cout << "QUE RAIVA\n";
    Node n_u = PushRelabel::getGraphNode(u);
    //const int INF = 10000000;
    // Linear loop to find the "v" nodes with the smallest height (future push contenders)
    for(int i=0;i<adj_list.size();i++) {
	int neighbor = adj_list[i].destNode_;
	int uv_current_c = adj_list[i].capacity_ - adj_list[i].currentFlow_;
	Node v = PushRelabel::getGraphNode(neighbor);
	std::pair<int,int> contender(neighbor, i);
	std::cout << v.h_ << " < " << v.h_ << "\n";
	std::cout << v.h_ << " >= " << n_u.h_ << "\n";
	std::cout << uv_current_c << " >  0" << "\n";
	if(v.h_ < min_h and v.h_ >= n_u.h_ and uv_current_c > 0) { 
	    min_h = v.h_;
            n_u.priority_.clear();
	    n_u.priority_.push_back(contender);
	    /*for(int i=0;i<n_u.priority_.size();i++)
                std::cout << n_u.priority_[i].first << "\n";*/
	    //std::cout << "achei contender:\n" << contender.first << "\n";
	    //std::cout << "min eh agora: " << min_h << "\n";
	    //n_u.h_ = min_h+1;
	}
	else if(v.h_ == min_h and uv_current_c > 0)
	    n_u.priority_.push_back(contender);
    }
    if(min_h < INF) // tem que ver a primeura vez q isso acontece
        n_u.h_ = min_h+1;
    else // nao devia aconteder... tem excesso e eh o mais alto... Faltando algo em push
	std::cout << "eitaaa preula\n";
   // n_u.h_ = min_h+1;
    //std::cout << "H\n";
    //std::cout << n_u.h_ << "\n";
    //std::cout << "Excess\n";
    //std::cout << n_u.excessFlow_ << "\n";
    //std::cout << "Conteders: \n";
    //for(int i=0;i<n_u.priority_.size();i++)
    //    std::cout << n_u.priority_[i].first << "\n";*/
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
	if(n_u.h_ - v.h_ == 1 and out_edges[i].capacity_-out_edges[i].currentFlow_ > 0) 
	    n_u.priority_.push_back(contender);
	/*if(n_u.h_ > v.h_ and out_edges[i].capacity_-out_edges[i].currentFlow_ > 0)
	    n_u.priority_.push_back(contender);*/
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
        //std::cout << "i will add this to a c (pre): " << uv.currentFlow_ << "\n";
        bool has_edge = false;
	int v = uv.destNode_;
	std::pair<Node, std::vector<Edge>> full_adj = PushRelabel::getAdjacencyList(v);
        std::vector<Edge> adj_list = full_adj.second;
        for(int j=0;j<adj_list.size();j++) {
            if(adj_list[j].destNode_ == s) {
		Edge vu = PushRelabel::getGraphEdge(v, s, j);
                //vu.capacity_ += uv.currentFlow_;
                vu.currentFlow_ -= uv.currentFlow_;
		PushRelabel::setGraphEdge(v, s, vu, j);
                //adj_list[j].capacity_ += uv.currentFlow_;
                //adj_list[j].currentFlow_ -= uv.currentFlow_;
		has_edge =  true;
		break;
            }
        }
        if(!has_edge)
	    PushRelabel::addEdge(out_edges[i].destNode_, s, uv.currentFlow_);

	//int v = out_edges[i].destNode_;
	Node n_v = PushRelabel::getGraphNode(v);
	n_v.excessFlow_ = uv.currentFlow_;
	PushRelabel::setGraphNode(n_v, v);
    }
}

int PushRelabel::getMaxFlow(int s, int t) {
    Node n_s = PushRelabel::getGraphNode(s);
    n_s.h_ = PushRelabel::getNumberOfNodes();
    PushRelabel::setGraphNode(n_s, s);
    PushRelabel::preFlow(s);
    bool active_exists = true;
    int max_flow = 0;

    while(active_exists) {
        int u = -1;
	int tallest_h = -1;
	std::vector<std::pair<Node,std::vector<Edge>>> graph;
	graph = PushRelabel::getGraph();
	
	// Buscar linear por nodo que tem excesso com maior altura
	for(int i=0;i<PushRelabel::getNumberOfNodes();i++) {
	    std::cout << "----";
	    std::cout << graph[i].first.excessFlow_ << "\n";
	    if(i != s and i != t) {
		//std::cout <<  "1\n";
	        if(graph[i].first.excessFlow_ > 0) {
		    //std::cout << "2\n";
	            if(graph[i].first.h_ > tallest_h) {
			//std::cout << "3\n";
		        u = i;
		        tallest_h = graph[i].first.h_;
		    }
	        }
	    }
	}
       
	std::cout << "Active u: " << u << "\n";
	if(u != -1) {
            PushRelabel::buildAlreadyBelow(u);
	    Node u_n = PushRelabel::getGraphNode(u);
            while(uIsActive(u_n)) { // while u still has excess to give
		//std::cout << "muahahaha\n";
		// u_n = PushRelabel::getGraphNode(u);
	        if(!u_n.priority_.empty()) { 
		    //std::cout << "ueeee\n";
	            bool push_done;
		    int sz = u_n.priority_.size();

		    push_done = PushRelabel::push(u, u_n.priority_[sz-1].first, u_n.priority_[sz-1].second);
	            
		    if(push_done) {
			u_n = PushRelabel::getGraphNode(u);
		        u_n.priority_.pop_back();
		        PushRelabel::setGraphNode(u_n, u);
		    }
		    else
	                std::cout << "There is something wrong\n";
	        }

	        else {
		    // std::cout << "ataaa\n";
		    PushRelabel::relabel(u);
		}
	        u_n = PushRelabel::getGraphNode(u);
		std::cout << "excesso: " << u_n.excessFlow_ << "\n";
	    }
	    //std::cout <<  "sai do loop tenebroso\n";
	}
	else
            active_exists = false;
    }
    
    /*std::pair<Node,std::vector<Edge>> adj_list = PushRelabel::getAdjacencyList(t);
    std::vector<Edge> out_edges = adj_list.second;
    for(int i=0;i<out_edges.size();i++) {
        Edge uv = PushRelabel::getGraphEdge(t, 0, i); // replace zero later
        max_flow += uv.capacity_;
    }*/
    
    Node n_t = PushRelabel::getGraphNode(t);
    max_flow = n_t.excessFlow_;
    return max_flow;
    /* retorna o excessFlow do t*/
    // na real nao, t eh so para organizacao do grafo, nao eh necessario... Eu acho
    // retorno o current flow dos out edges de s
}


void PushRelabel::setS(unsigned s) { PushRelabel::s_ = s; }


void PushRelabel::setT(unsigned t) { PushRelabel::t_ = t; }
