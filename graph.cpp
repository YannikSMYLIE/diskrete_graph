// graph.cpp (Implementation of Class Graph)

#include <fstream>
#include <sstream>
#include <limits>
#include "graph.h"

const Graph::NodeId Graph::invalid_node = -1;
const double Graph::infinite_weight = std::numeric_limits<double>::max();


void Graph::add_nodes(NodeId num_new_nodes)
{
    _nodes.resize((int)num_nodes() + num_new_nodes);
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
void Graph::Node::markEdge(NodeId to) {
    for(int i = 0; i < _neighbors.size(); i++) {
        if(_neighbors.at(i).id() == to && !_neighbors.at(i).isMarked()) {
            _neighbors.at(i).setMarked(true);
            break;
        }
    }
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
bool Graph::Neighbor::isMarked() const {
    return _marked;
}
void Graph::Neighbor::setMarked(bool marked) {
    _marked = marked;
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
    if(dirtype == Graph::undirected) {
        // Prüfen ob Knotengewichte gerade
        for (auto nodeid = 0; nodeid < num_nodes(); nodeid++) {
            if(_nodes[nodeid].adjacent_nodes().size() % 2 != 0) {
                return false;
            }
        }
    } else {
        // Prüfen ob jeder Knoten genausoviele Eingangs wie Ausgangsknoten hat
        int edge_count [Graph::num_nodes()] = {0};
        for (auto nodeid = 0; nodeid < num_nodes(); nodeid++) {
            edge_count[nodeid] += _nodes[nodeid].adjacent_nodes().size();
            for(auto edgeid = 0; edgeid < _nodes[nodeid].adjacent_nodes().size(); edgeid++) {
                edge_count[_nodes[nodeid].adjacent_nodes().at(edgeid).id()] -= 1;
            }
        }
        for(auto i = 0; i < Graph::num_nodes(); i++) {
            if(edge_count[i] != 0) {
                return false;
            }
        }
    }
    // Prüfen ob nur eine Zusammenhangskomponente (mit Breitensuche)
    int i = 1;
    std::vector<Graph::Node> toCheck;
    toCheck.push_back(_nodes[0]);
    _nodes[0].setMarked(true);

    while(!toCheck.empty()) {
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
/**
 * Die Funktion prüft ob es eine Eulertour gibt und gibt diese als Text aus. Sollte es keine Eulertour geben wird ein Fehler ausgegeben.
 */
void Graph::findEulertour() {
    if(!Graph::isEulersch()) {
        std::cout << "Der Graph ist nicht eulersch und enthält daher keine Eulertour." << std::endl;
        return;
    }

    // Zuerst speichern wir wieviele ausgehende Kanten jeder Knoten hat damit wir wissen welche wir noch abarbeiten müssen und welche Kanten bereits bearbeitet wurden!
    int edge_count [Graph::num_nodes()];
    for(int i = 0; i < Graph::num_nodes(); i++) {
        edge_count[i] = _nodes[i].adjacent_nodes().size();
    }

    // Wir brauchen einen Stack um uns zu merken welche Kante wir gerade ansehen
    std::stack<int> currentPath;
    // Außerdem brauchen wir einen Stack um uns den ganzen Weg zu merken
    std::stack<int> finalPath;
    // Und wir legen sofort mal den ersten Knoten auf den Stack
    currentPath.push(0);

    while(!currentPath.empty()) {
        Graph::Node &currentNode = _nodes[currentPath.top()];
        int currentNodeId = currentPath.top();
        if(edge_count[currentPath.top()] > 0) { // Dieser Knoten hat noch Kanten die abgearbeitet werden müssen
            // Id des nächsten Knotens finden
            int nextNodeId = -1;
            if(dirtype == Graph::directed) {
                // Bei gerichteten Graphen können wir einfach die nächste ausgehende Kante nehmen
                nextNodeId = currentNode.adjacent_nodes().at(edge_count[currentPath.top()] - 1).id();
            } else {
                // Finde eine Kante welche noch nicht bearbeitet wurde
                for(auto i = 0; i < currentNode.adjacent_nodes().size(); i++) {
                    if(!currentNode.adjacent_nodes().at(i).isMarked()) {
                        currentNode.markEdge(currentNode.adjacent_nodes().at(i).id());
                        _nodes[currentNode.adjacent_nodes().at(i).id()].markEdge(currentNodeId);
                        nextNodeId = currentNode.adjacent_nodes().at(i).id();
                        edge_count[nextNodeId]--; // Auch hier die Kantenanzahl reduzieren
                        break;
                    }
                }
            }
            // Kantenanzahl reduzieren
            edge_count[currentPath.top()]--;
            // Nächsten Knoten in Liste eintragen
            currentPath.push(nextNodeId);
        } else { // Alle Kanten dieses Knotens wurden abgearbeitet
            // Knoten als besucht speichern
            finalPath.push((int)currentPath.top());
            // Knoten aus Stack entfernen
            currentPath.pop();
        }
    }

    std::cout << "Folgende Eulertour wurde gefunden:" << std::endl;
    while(!finalPath.empty()) {
        int curNode = finalPath.top();
        std::cout << curNode << std::endl;
        finalPath.pop();
    }
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
