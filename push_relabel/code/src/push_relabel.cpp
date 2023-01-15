#include <iostream>
#include "../include/push_relabel.h"

using namescape PushRelabel;

bool uIsActive(Node u) {
    return (u_node.excessFlow_ > 0) ? true : false;
}

bool vIsRightBelow(Node u, Node v) {
    return (u.h_ - v.h_ == 1) ? true : false;  
}

// essa funcao so pode ser chamada se achou um edge entre u e v(feito antes já), u tava na frente da linha da prioridade
// E A CAPACIDADE ENTRE U e V EH MAIOR QUE ZERO (ARESTA AINDA EXISTE)
bool push(int u, int v, int uv_idx) {
    bool possible_push;
    Node u_n = getGraphNode(u);
    Node v_n = getGraphNode(v);
    
    possible_push = uIsActive(u_n) & vIsRightBelow(u_n, v_n); // capacidade nao pode ser zero (pode ser em grafo residual se consumiu tudo)

    if(possible_push) {
	Edge uv = getGraphEdge(u, v, uv_idx); 
        Edge updated_uv;                    
	Node new_u = u_n;
	int extra_flow = min(uv.capacity_, u_node.excessFlow_);

	new_u.excessFlow_ -= extra_flow;
	setGraphNode(new_u);

	updated_uv.currentFlow_ = uv.currentFlow_ + extra_flow;
	updated_uv.capacity_ = uv.capacity_ - extra_flow;
	updated_uv.destNode_ = v;
	setGraphEdge(u, v, updated_uv, uv_idx); 
        
        addEdge(v, u, extra_flow);

	return true;
    }

    return false;
}

void relabel(int u) {
    std::pair<Node, std::vector<Edge>> adj_list;
    adj_list = getAdjacencyList(u);
    for(int i=0;i<adj_list.size(),i++) {
        // merge usando o array com pair
    }
    for(int i=0;i<adj_list.size(),i++) {
        // preenche lista de contenders (v e indice de onde eles estao na adj list)
    }
    // preciso criar o array de pair(soh nesse escopo), a lista, pair, de contenders (em cada nodo)
}

void setS(unsigned s) { s_ = s; }

void setT(unsigned t) { t_ = t; }
