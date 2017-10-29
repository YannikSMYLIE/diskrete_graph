//
// Created by Yannik on 26.10.2017.
//
#include "graph.h"

Graph::Node::Node() {
    _marked = false;
}
void Graph::Node::addEdge(Graph::NodeId from, Graph::NodeId to, Graph::Edge *edge)
{
    _neighbors.push_back(Graph::Neighbor(from, to, edge));
}


bool Graph::Node::mark() {
    if(_marked) {
        return false;
    }
    _marked = true;
    return true;
}
void Graph::Node::unmark() {
    _marked = false;
}
void Graph::Node::unmark_all() {
    for (auto neighborId = 0; neighborId < _neighbors.size(); neighborId++) {
        get_edge_at(neighborId) -> unmark();
    }
}
bool Graph::Node::isMarked() const {
    return _marked;
}


const std::vector<Graph::Neighbor> & Graph::Node::adjacent_nodes() const
{
    return _neighbors;
}
Graph::Neighbor * Graph::Node::get_edge_to(Graph::NodeId to) {
    for(int i = 0; i < _neighbors.size(); i++) {
        if(_neighbors.at(i).to() == to) {
            return &_neighbors.at(i);
        }
    }
}
Graph::Neighbor * Graph::Node::get_edge_at(int index) {
    if(index < _neighbors.size()) {
        return &_neighbors[index];
    }
}







/**
void Graph::Node::markEdge(NodeId to) {
    for(int i = 0; i < _neighbors.size(); i++) {
        if(_neighbors.at(i).id() == to && !_neighbors.at(i).isMarked()) {
            _neighbors.at(i).setMarked(true);
            break;
        }
    }
}*/
