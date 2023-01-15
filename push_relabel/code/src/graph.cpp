#include "../include/graph.h"

bool Graph::addEdge(int u, int v, int w) {

    if(u < numberOfNodes_ and v < numberOfNodes_) {
        Graph::graph_[u].second.push_back(Edge(v, w));
        return true;
    }
    return false;
}

bool Graph::setGraphNode(Node updated, int node_number) {

    if(node_number < numberOfNodes_) { 
        Graph::graph_[node_number].first = updated;
        return true;
    }
    return false;
}

Node Graph::getGraphNode(int node_number) {

    std::pair<Node, std::vector<Edge>> node_pair = Graph::getAdjacencyList(node_number);
    Node selected_node = node_pair.first;
    
    return selected_node;
}

bool Graph::setGraphEdge(int u, int v, Edge updated, int uv_idx) {
    //std::pair<Node, std::vector<Edge>> adj_list;
    //adj_list = Graph::getAdjacencyList(u);
    if(uv_idx != -1) {
	Graph::graph_[u].second[uv_idx] = updated;
	return true;
    }
    return false;
}

Edge Graph::getGraphEdge(int u, int v, int uv_idx) {
    std::pair<Node, std::vector<Edge>> adj_list;
    adj_list = Graph::getAdjacencyList(u);
    if(uv_idx != -1) {
        return adj_list.second[uv_idx];
    }
}

void Graph::removeGraphEdge(int u, int v, int uv_idx) {
    //std::pair<Node, std::vector<Edge>> adj_list;
    //adj_list = Graph::getAdjacencyList(u);
    if(uv_idx != -1) {
        Edge tmp = graph_[u].second[graph_[u].second.size()-1];
	graph_[u].second[graph_[u].second.size()-1] = graph_[u].second[uv_idx];
	graph_[u].second[uv_idx] = tmp;
	graph_[u].second.pop_back();
    }
}

std::pair<Node, std::vector<Edge>> Graph::getAdjacencyList(int node) {
    return Graph::graph_[node];
}

/*std::vector<Edge> &Graph::getRefToAdjList(int node) {
    std::pair<Node, std::vector<Edge>> adj_list;
    adj_list = getAdjacencyList(node);
    return adj_list.second;
}*/
