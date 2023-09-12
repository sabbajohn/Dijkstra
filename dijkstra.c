#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 1000
#define INFINITY 9999

// Structure to represent a vertex in the graph
typedef struct {
    int distance;
    bool visited;
} Vertex;

// Function to find the vertex with the minimum distance
int minDistance(Vertex *vertices, int numVertices) {
    int min = INFINITY;
    int minIndex;
    
    for (int v = 0; v < numVertices; v++) {
        if (!vertices[v].visited && vertices[v].distance <= min) {
            min = vertices[v].distance;
            minIndex = v;
        }
    }
    
    return minIndex;
}

// Function to print the shortest path from source to destination
void printShortestPath(int *parent, int dest) {
    if (parent[dest] == -1) {
        printf("%d ", dest);
        return;
    }
    
    printShortestPath(parent, parent[dest]);
    printf("%d ", dest);
}

// Function to implement Dijkstra's shortest path algorithm
void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices, int src, int dest) {
    Vertex vertices[MAX_VERTICES];
    int parent[MAX_VERTICES];
    
    // Initialize all vertices with maximum distance and not visited
    for (int i = 0; i < numVertices; i++) {
        vertices[i].distance = INFINITY;
        vertices[i].visited = false;
        parent[i] = -1;
    }
    
    // Distance from source to itself is always 0
    vertices[src].distance = 0;
    
    // Find shortest path for all vertices
    for (int count = 0; count < numVertices - 1; count++) {
        int u = minDistance(vertices, numVertices);
        
        vertices[u].visited = true;
        
        for (int v = 0; v < numVertices; v++) {
            if (!vertices[v].visited && graph[u][v] && vertices[u].distance + graph[u][v] < vertices[v].distance) {
                vertices[v].distance = vertices[u].distance + graph[u][v];
                parent[v] = u;
            }
        }
    }
    
    printf("Shortest path from %d to %d: ", src, dest);
    printShortestPath(parent, dest);
    printf("\n");
}

// Main function
int main() {
    int graph[MAX_VERTICES][MAX_VERTICES], numVertices;
    int src, dest;

    printf("Enter the number of vertices: ");
    scanf("%d", &numVertices);

    printf("Enter the adjacency matrix:\n");
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    printf("Enter the source vertex: ");
    scanf("%d", &src);

    printf("Enter the destination vertex: ");
    scanf("%d", &dest);

    dijkstra(graph, numVertices, src, dest);

    return 0;
}
