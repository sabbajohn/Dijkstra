#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>               // Include the necessary header file
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_VERTICES 9

#ifdef INFINITY
#undef INFINITY
#endif
#define INFINITY 9999

typedef struct{
    int x;
    int y;
} Point;


// Structure to represent a vertex in the graph
typedef struct {
    int distance;
    bool visited;
    Point point;
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
void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices, int src, int dest, SDL_Renderer* renderer) {
    Vertex vertices[MAX_VERTICES];
    int parent[MAX_VERTICES];
    
    // Inicializa todos os vértices com distância máxima e não visitados
    for (int i = 0; i < numVertices; i++) {
        vertices[i].distance = INFINITY;
        vertices[i].visited = false;
        vertices[i].point.x = rand() %500 + 50;
        vertices[i].point.y = rand() %500 + 50;
        parent[i] = -1;
        
        if (i == src){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        }else if (i == dest){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }else
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        
        SDL_Rect rect = {vertices[i].point.x, vertices[i].point.y, 10, 10};
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        usleep(100000);
    }
    
    // A distância do vértice de origem para si mesmo é sempre 0
    vertices[src].distance = 0;
    
    // Encontra o caminho mais curto para todos os vértices
    for (int count = 0; count < numVertices - 1; count++) {
        int u = minDistance(vertices, numVertices);
        
        vertices[u].visited = true;
        if (u != dest && u != src){
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Cor do vértice visitado
            SDL_Rect rect = {vertices[u].point.x, vertices[u].point.y, 10, 10};
            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderPresent(renderer);
        }
        // Renderiza o grafo atualizado
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (graph[i][j] > 0 && graph[i][j] < INFINITY) {
                    if ((vertices[i].visited && vertices[j].visited) || (i == src && j == dest)) {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Cor da linha se ambos os vértices forem visitados
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Cor da linha se apenas um dos vértices for visitado
                    }
                    
                    SDL_RenderDrawLine(renderer, vertices[i].point.x, vertices[i].point.y, vertices[j].point.x, vertices[j].point.y);
                }
            }
        }
        
        
        SDL_RenderPresent(renderer);
        usleep(100000);
        
        for (int v = 0; v < numVertices; v++) {
            if (!vertices[v].visited && graph[u][v] && vertices[u].distance + graph[u][v] < vertices[v].distance) {
                vertices[v].distance = vertices[u].distance + graph[u][v];
                parent[v] = u;
            }
        }
    }
    
    printf("Caminho mais curto de %d para %d: ", src, dest);
    printShortestPath(parent, dest);
    printf("\n");
}




int main() {
    int graph[MAX_VERTICES][MAX_VERTICES] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };;
    int numVertices = 9;
    int src, dest;

    // printf("Enter the number of vertices: ");
    // scanf("%d", &numVertices);

    // printf("Enter the adjacency matrix:\n");
    // for (int i = 0; i < numVertices; i++) {
    //     for (int j = 0; j < numVertices; j++) {
    //         scanf("%d", &graph[i][j]);
    //     }
    // }

    printf("Enter the source vertex: ");
    scanf("%d", &src);

    printf("Enter the destination vertex: ");
    scanf("%d", &dest);

    // Initialize SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    dijkstra(graph, numVertices, src, dest, renderer);

    // Handle events until the user closes the window
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
