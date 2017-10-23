// graph.cpp (Implementation of Class Graph)

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "graph.h"

const Graph::NodeId Graph::invalid_node = -1;
const double Graph::infinite_weight = std::numeric_limits<double>::max();


void Graph::add_nodes(NodeId num_new_nodes)
{
    _nodes.resize(num_nodes() + num_new_nodes);
}

Graph::Neighbor::Neighbor(Graph::NodeId n, double w): _id(n), _edge_weight(w) {}

Graph::Graph(NodeId num, DirType dtype): dirtype(dtype), _nodes(num) {}

void Graph::add_edge(NodeId tail, NodeId head, double weight)
{
    if (tail >= num_nodes() or tail < 0 or head >= num_nodes() or head < 0) {
        throw std::runtime_error("Edge cannot be added due to undefined endpoint.");
    }
    _nodes[tail].add_neighbor(head, weight);
    if (dirtype == Graph::undirected) {
        _nodes[head].add_neighbor(tail, weight);
    }
}

void Graph::Node::add_neighbor(Graph::NodeId nodeid, double weight)
{
    _neighbors.push_back(Graph::Neighbor(nodeid, weight));
}
void Graph::Node::setMarked(bool marked) {
    _marked = marked;
}
bool Graph::Node::isMarked() {
    return _marked;
}

const std::vector<Graph::Neighbor> & Graph::Node::adjacent_nodes() const
{
    return _neighbors;
}

Graph::NodeId Graph::num_nodes() const
{
    return _nodes.size();
}

const Graph::Node & Graph::get_node(NodeId node) const
{
    if (node < 0 or node >= static_cast<int>(_nodes.size())) {
        throw std::runtime_error("Invalid nodeid in Graph::get_node.");
    }
    return _nodes[node];
}

Graph::NodeId Graph::Neighbor::id() const
{
    return _id;
}

double Graph::Neighbor::edge_weight() const
{
    return _edge_weight;
}

void Graph::print() const
{
    if (dirtype == Graph::directed) {
        std::cout << "Digraph ";
    } else {
        std::cout << "Undirected graph ";
    }
    std::cout << "with " << num_nodes() << " vertices, numbered 0,...,"
              << num_nodes() - 1 << ".\n";

    for (auto nodeid = 0; nodeid < num_nodes(); ++nodeid) {
        std::cout << "The following edges are ";
        if (dirtype == Graph::directed) {
            std::cout << "leaving";
        } else {
            std::cout << "incident to";
        }
        std::cout << " vertex " << nodeid << ":\n";
        for (auto neighbor: _nodes[nodeid].adjacent_nodes()) {
            std::cout << nodeid << " - " << neighbor.id()
                      << " weight = " << neighbor.edge_weight() << "\n";
        }
    }
}

bool Graph::isEulersch() {
    // Prüfen ob Knotengewicht gerade
    for (auto nodeid = 0; nodeid < num_nodes(); nodeid++) {
        if(_nodes[nodeid].adjacent_nodes().size() % 2 != 0) {
            return false;
        }
    }
    // Prüfen ob nur eine Zusammenhangskomponente (mit Breitensuche)
    int i = 1;
    std::vector<Graph::Node> toCheck;
    toCheck.push_back(_nodes[0]);
    _nodes[0].setMarked(true);

    while(toCheck.size() > 0) {
        Graph::Node node = toCheck.back();
        toCheck.pop_back();

        for (auto neighbor: node.adjacent_nodes()) {
            if(!_nodes[neighbor.id()].isMarked()) {
                i++;
                _nodes[neighbor.id()].setMarked(true);
                toCheck.push_back(_nodes[neighbor.id()]);
            }
        }
    }
    return i == this->num_nodes();
}

Graph::Graph(char const * filename, DirType dtype): dirtype(dtype)
{
    std::ifstream file(filename);                             // open file
    if (not file) {
        throw std::runtime_error("Cannot open file.");
    }

    Graph::NodeId num = 0;
    std::string line;
    std::getline(file, line);                 // get first line of file
    std::stringstream ss(line);               // convert line to a stringstream
    ss >> num;                                // for which we can use >>
    if (not ss) {
        throw std::runtime_error("Invalid file format.");
    }
    add_nodes(num);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Graph::NodeId head, tail;
        ss >> tail >> head;
        if (not ss) {
            throw std::runtime_error("Invalid file format.");
        }
        double weight = 1.0;
        ss >> weight;
        if (tail != head) {
            add_edge(tail, head, weight);
        }
        else {
            throw std::runtime_error("Invalid file format: loops not allowed.");
        }
    }
}
