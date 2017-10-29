//
// Created by Yannik on 26.10.2017.
//
#include "graph.h"

// Funktionen der Klasse Edge
Graph::Edge::Edge(double w) {
    _edge_weight = w;
    _marked = false;
}
double Graph::Edge::getWeight() const {
    return _edge_weight;
}
bool Graph::Edge::mark() {
    if(_marked) {
        return false;
    }
    _marked = true;
    return true;
}
void Graph::Edge::unmark() {
    _marked = false;
}
bool Graph::Edge::isMarked() const {
    return _marked;
}

// Funktionen der Klasse Neighbor
Graph::Neighbor::Neighbor(Graph::NodeId from, Graph::NodeId to, Graph::Edge *edge) {
    _from = from;
    _to = to;
    _edge = edge;
}
Graph::NodeId Graph::Neighbor::from() const {
    return _from;
}
Graph::NodeId Graph::Neighbor::to() const {
    return _to;
}
double Graph::Neighbor::getWeight() const {
    return (*_edge).getWeight();
}
bool Graph::Neighbor::mark() {
    return (*_edge).mark();
}
void Graph::Neighbor::unmark() {
    (*_edge).unmark();
}
bool Graph::Neighbor::isMarked() const {
    return (*_edge).isMarked();
}