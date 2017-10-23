#include <iostream>
#include "graph.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    Graph g("C:/Users/Yannik/graph.txt", Graph::undirected);
    //g.print();
    g.findEulertour();
    return 0;
}