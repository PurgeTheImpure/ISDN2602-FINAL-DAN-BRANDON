#ifndef algo_H_
#define algo_H_
#include <Arduino.h>
//#include <ArduinoSTL.h>
#include "Pinout.hpp"

struct Edge {
    int to;
    int weight;
};

class Graph {
public:
    Graph(int numNodes);
    void addEdge(int from, int to, int weight);
    void dijkstra(int start, int end, int* path, int& pathLength);
    void setupGraph();

private:
    int numNodes;
    static const int MAX_EDGES = 20; // Adjust as needed
    Edge adjList[14][MAX_EDGES];
    int edgeCount[14];
    const int INF = 10000; // Define a large enough value as infinity
};


#endif