#include <iostream>
#include "graph.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    Graph g("C:/Users/Yannik/graph.txt", Graph::undirected);
    g.print();
    if(g.isEulersch()) {
        std::cout << "Der Graph ist eulersch!" << std::endl;
    } else {
        std::cout << "Der Graph ist nicht eulersch!" << std::endl;
    }
    return 0;
}