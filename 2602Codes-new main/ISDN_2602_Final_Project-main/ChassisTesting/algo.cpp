#include "algo.hpp"

Graph::Graph(int numNodes) : numNodes(numNodes) {
    for (int i = 0; i < numNodes; ++i) {
        edgeCount[i] = 0;
    }
}

void Graph::addEdge(int from, int to, int weight) {
    adjList[from][edgeCount[from]++] = {to, weight};
    adjList[to][edgeCount[to]++] = {from, weight}; // Assuming an undirected graph
}

void Graph::setupGraph() {
    // Add edges with distances (example data)
    addEdge(0, 1, 66);
    addEdge(0, 2, 39);
    addEdge(0, 4, 86);
    addEdge(1, 2, 46);
    addEdge(1, 5, 30);
    addEdge(2, 3, 27);
    addEdge(3, 4, 20);
    addEdge(3, 6, 38);
    addEdge(4, 12, 74);
    addEdge(5, 10, 35);
    addEdge(5, 11, 70);
    addEdge(6, 7, 23);
    addEdge(6, 8, 14);
    addEdge(8, 9, 23);
    addEdge(8, 13, 52);
    addEdge(10, 11, 35);
    addEdge(10, 13, 103);
    addEdge(11, 12, 101);
    addEdge(12, 13, 92);
    // Add other edges as needed
}

void Graph::dijkstra(int start, int end, int* path, int& pathLength) {
    int dist[14];
    int prev[14];
    bool visited[14];

    for (int i = 0; i < numNodes; ++i) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }

    dist[start] = 0;

    for (int i = 0; i < numNodes; ++i) {
        int u = -1;
        for (int j = 0; j < numNodes; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INF) break;
        visited[u] = true;

        for (int k = 0; k < edgeCount[u]; ++k) {
            int v = adjList[u][k].to;
            int weight = adjList[u][k].weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    // Reconstruct the shortest path
    int currentNode = end;
    pathLength = 0;
    while (currentNode != -1) {
        path[pathLength++] = currentNode;
        currentNode = prev[currentNode];
    }

    // Reverse the path to get the correct order
    for (int i = 0; i < pathLength / 2; ++i) {
        int temp = path[i];
        path[i] = path[pathLength - 1 - i];
        path[pathLength - 1 - i] = temp;
    }

    // Check if the path is valid
    if (pathLength == 1 && path[0] != start) {
        pathLength = 0; // No path found
    }

    


}