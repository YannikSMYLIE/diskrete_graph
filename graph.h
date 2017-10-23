// graph.h (Declaration of Class Graph)
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

class Graph {
public:
    using NodeId = int;  // vertices are numbered 0,...,num_nodes()-1

    class Neighbor {
    public:
        Neighbor(Graph::NodeId n, double w);
        double edge_weight() const;
        Graph::NodeId id() const;
    private:
        Graph::NodeId _id;
        double _edge_weight;
    };

    class Node {
    public:
        void add_neighbor(Graph::NodeId nodeid, double weight);
        void setMarked(bool marked);
        bool isMarked();
        const std::vector<Neighbor> & adjacent_nodes() const;
    private:
        std::vector<Neighbor> _neighbors;
        bool _marked;
    };

    enum DirType {directed, undirected};  // enum defines a type with possible values
    Graph(NodeId num_nodes, DirType dirtype);
    Graph(char const* filename, DirType dirtype);

    void add_nodes(NodeId num_new_nodes);
    void add_edge(NodeId tail, NodeId head, double weight = 1.0);

    NodeId num_nodes() const;
    const Node & get_node(NodeId) const;
    void print() const;

    bool isEulersch();

    const DirType dirtype;
    static const NodeId invalid_node;
    static const double infinite_weight;

private:
    std::vector<Node> _nodes;
};

#endif // GRAPH_H
