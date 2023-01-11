#include "../include/graph.h"

bool Graph::addEdge(int u, int v, int w) {

    if(u < numberOfNodes_ && v < numberOfNodes_) {
        Graph::graph_[u].second().push_back(Edge(v, w));
        return true;
    }
    return false;
}

bool Graph::setGraphNode(Node updated, int node_number) {

    if(node_number < numberOfNodes_) { 
        Graph::graph_[node_number].first() = updated;
        return true;
    }
    return false;
}

Node Graph::getGraphNode(int node_number) {

    std::pair<Node, std::vector<Edge>> node_pair = Graph::graph_[node_number];
    Node selected_node = node_pair.first();
    
    return selected_node;
}

bool Graph::setGraphEdge(int u, int v, Edge updated) {
    return false;
}

Edge Graph::getGraphEdge(int u, int v) {

}

std::pair<Node, std::vector<Edge>> Graph::getAdjacencyList(int node) {
    return Graph::graph_[node];
}


