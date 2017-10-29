// graph.cpp (Implementation of Class Graph)

#include <fstream>
#include <sstream>
#include <limits>
#include "graph.h"

// Unklar was der Quatsch hier sein soll
const Graph::NodeId Graph::invalid_node = -1;
const double Graph::infinite_weight = std::numeric_limits<double>::max();

/**
 * Erzeugt einen Graphen.
 * @param num int Anzahl der Knoten die der Graph enthalten soll.
 * @param dtype DirType Angabe ob der Graph gerichtet oder ungerichtet ist.
 */
Graph::Graph(int num, DirType dtype): dirtype(dtype), _nodes(num) {}




/**
 * Fügt dem Graphen eine Kante hinzu.
 * @param tail Die Id des Startknotens.
 * @param head  Die Id des Zielknotens.
 * @param weight Das Gewicht der Kante.
 */
void Graph::add_edge(NodeId tail, NodeId head, double weight)
{
    if (tail >= num_nodes() or tail < 0 or head >= num_nodes() or head < 0) {
        throw std::runtime_error("Edge cannot be added due to undefined endpoint.");
    }

    Graph::Edge *edge = new Graph::Edge(weight);
    _nodes[tail].addEdge(tail, head, edge);
    if (dirtype == Graph::undirected) {
        _nodes[head].addEdge(head, tail, edge);
    }
}

/**
 * Ermittelt wieviele Knoten im Graph vorhanden sind.
 * @return int Die Anzahl der Knoten.
 */
int Graph::num_nodes() const
{
    return _nodes.size();
}

/**
 * Findet einen Knoten im Graphen.
 * @param node NodeId Die Id des zu findenden Knoten.
 * @return Graph::Node Der Knoten.
 */
Graph::Node * Graph::get_node(NodeId node)
{
    if (node < 0 or node >= num_nodes()) {
        throw std::runtime_error("Invalid nodeid in Graph::get_node.");
    }
    return &_nodes[node];
}

/**
 * Gibt den Graphen in der Konsole aus
 */

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
            std::cout << neighbor.from() << " - " << neighbor.to()
                      << " weight = " << (neighbor).getWeight();
            if((neighbor).isMarked()) {
                std::cout << " - markiert\n";
            } else {
                std::cout << " - nicht markiert\n";
            }
        }
    }
}

void Graph::add_nodes(NodeId num_new_nodes)
{
    _nodes.resize((int)num_nodes() + num_new_nodes);
}








/**
 * Prüft ob der Graph eulersch ist oder nicht
 * @return
 */
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
                edge_count[_nodes[nodeid].adjacent_nodes().at(edgeid).to()] -= 1;
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
    _nodes[0].mark();

    while(!toCheck.empty()) {
        Graph::Node node = toCheck.back();
        toCheck.pop_back();

        for (auto neighbor: node.adjacent_nodes()) {
            if(_nodes[neighbor.to()].mark()) {
                i++;
                toCheck.push_back(_nodes[neighbor.to()]);
            }
        }
    }

    Graph::unmark_all(true);
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
                nextNodeId = currentNode.adjacent_nodes().at(edge_count[currentPath.top()] - 1).to();
            } else {
                // Finde eine Kante welche noch nicht bearbeitet wurde
                for(auto i = 0; i < currentNode.adjacent_nodes().size(); i++) {
                    if(!currentNode.adjacent_nodes().at(i).isMarked()) {
                        Graph::Neighbor * neighbor = currentNode.get_edge_at(i);
                        neighbor -> mark();
                        edge_count[neighbor -> to()]--; // Auch hier die Kantenanzahl reduzieren
                        nextNodeId = neighbor -> to();
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

    Graph::unmark_all(false);
}

void Graph::unmark_all(bool recursive) {
    for (auto nodeid = 0; nodeid < num_nodes(); nodeid++) {
        get_node(nodeid) -> unmark();
        if(recursive) {
            //get_node(nodeid) -> unmark_all();
        }
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
