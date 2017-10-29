#include <iostream>
#include "Graph/graph.h"

int main(int argc, char* argv[]) {
    for(int i = 7; i < 8; i++) {
        std::string filename = "C:/Users/Yannik/Vygen/inst_euler_" +std::to_string(i) + ".txt";
        Graph g(filename.c_str(), Graph::undirected);
        std::cout << std::endl << "Suche Eulertour für Instanz " << i << std::endl;
        g.findEulertour();
    }


    return 0;
}