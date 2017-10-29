// graph.h (Declaration of Class Graph)
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <stack>

class Graph {
public:
    using NodeId = int;  // vertices are numbered 0,...,num_nodes()-1

    /*
     * Hier kommen die Kanteklassen.
     * Edge:
     *      Enthält alle wichtigen Informationen einer Kante, wie z.B. ob sie markiert ist oder welches Gewicht sie hat.
     * Neighbor:
     *      Entspricht eine Richtung der Kante.
     *      Bei ungerichteten Graphen wird ein Neighbor für from und to angelegt (jeweils vertauscht). Bei gerichteten nur in eine Richtung.
     *      Durch verweis auf Edge kann eine Kante direkt in beide Richtungen manipuliert werden.
     */
    class Edge {
    public:
        Edge(double w);
        bool isMarked() const;
        bool mark();
        void unmark();
        double getWeight() const;
    private:
        bool _marked;
        double _edge_weight;
    };

    class Neighbor {
    public:
        Neighbor(Graph::NodeId from, Graph::NodeId to, Graph::Edge *edge);
        Graph::NodeId from() const;
        Graph::NodeId to() const;
        bool isMarked() const;
        bool mark();
        void unmark();
        double getWeight() const;
    private:
        Graph::NodeId _from;
        Graph::NodeId _to;
        Graph::Edge * _edge;
    };

    /*
     * Hier kommt die Knotenklasse
     */
    class Node {
    public:
        Node();
        void addEdge(Graph::NodeId from, Graph::NodeId to, Graph::Edge *edge);
        bool mark();
        void unmark();
        void unmark_all();
        bool isMarked() const;
        Graph::Neighbor * get_edge_to(Graph::NodeId to);
        Graph::Neighbor * get_edge_at(Graph::NodeId to);
        const std::vector<Graph::Neighbor> & adjacent_nodes() const;
    private:
        std::vector<Neighbor> _neighbors;
        bool _marked;
        NodeId _id;
    };

    /*
     * Hier kommt alles was die Graphenklasse kann.
     */
    enum DirType {directed, undirected};  // enum defines a type with possible values
    Graph(int num_nodes, DirType dirtype);
    Graph(char const* filename, DirType dirtype);

    void unmark_all(bool recursive = false);

    void add_nodes(NodeId num_new_nodes);
    void add_edge(NodeId tail, NodeId head, double weight = 1.0);

    int num_nodes() const;
    Graph::Node * get_node(NodeId);
    void print() const;

    bool isEulersch();
    void findEulertour();

    const DirType dirtype;
    static const NodeId invalid_node;
    static const double infinite_weight;

private:
    std::vector<Node> _nodes;
};

#endif // GRAPH_H
