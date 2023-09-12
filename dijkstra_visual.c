#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>               // Include the necessary header file
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAX_VERTICES 1000

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
void printShortestPath(int *parent, int dest, Vertex *vertices, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Shortest path color
    if (parent[dest] == -1) {
        printf("%d ", dest);
        return;
    }
    
    SDL_RenderDrawLine(renderer, vertices[dest].point.x, vertices[dest].point.y, vertices[parent[dest]].point.x, vertices[parent[dest]].point.y);
    SDL_RenderPresent(renderer);
    usleep(100000);
    printShortestPath(parent, parent[dest],vertices, renderer);
    printf("%d ", dest);
}

// Function to implement Dijkstra's shortest path algorithm
void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices, int src, int dest, SDL_Renderer* renderer) {
    Vertex vertices[MAX_VERTICES];
    int parent[MAX_VERTICES];
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("Arial.ttf", 24);
    char str[20];
    
    // Initialize all vertices with maximum distance and not visited
    for (int i = 0; i < numVertices; i++) {
        vertices[i].distance = INFINITY;
        vertices[i].visited = false;
        vertices[i].point.x = rand() %1000 + 50;
        vertices[i].point.y = rand() %640 + 50;
        parent[i] = -1;
        SDL_Color textColor;
        if (i == src){
            textColor = (SDL_Color) {0, 255, 0, 255};
        } else if(i == dest) {
            textColor = (SDL_Color) {255, 0, 0, 255};
        } else {
            textColor = (SDL_Color) {255, 255, 255, 255};
        }
        
        SDL_Rect rect = {vertices[i].point.x, vertices[i].point.y, 30, 30};
        
        sprintf(str, "%d", i);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_RenderCopyEx(renderer, textTexture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        SDL_RenderPresent(renderer);
    }
    
    // Distance from source to itself is always 0
    vertices[src].distance = 0;
    
    // Find shortest path for all vertices
    for (int count = 0; count < numVertices - 1; count++) {
        int u = minDistance(vertices, numVertices);
        
        vertices[u].visited = true;

        if (u != dest && u != src){
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 127); // Color of visited vertex
            
        }else if(u == dest){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127); // Color of destination vertex
        }else if (u == src){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 127); // Color of source vertex
        }
        SDL_Rect rect = {vertices[u].point.x, vertices[u].point.y, 10, 10};
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        
        for (int v = 0; v < numVertices; v++) {
            if (!vertices[v].visited && graph[u][v] && vertices[u].distance + graph[u][v] < vertices[v].distance) {
                vertices[v].distance = vertices[u].distance + graph[u][v];
                parent[v] = u;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
                SDL_RenderDrawLine(renderer, vertices[v].point.x, vertices[v].point.y, vertices[u].point.x, vertices[u].point.y);
            }
            
            SDL_RenderPresent(renderer);
            usleep(500);
        }
    }
    
    printf("Caminho mais curto de %d para %d: ", src, dest);
    printShortestPath(parent, dest, vertices, renderer);
    printf("\n");
}




int main() {
    int graph[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
    int src, dest;

    printf("Choose an option:\n");
    printf("1. Use a pre-defined graph\n");
    printf("2. Enter a graph manually\n");
    printf("3. Generate a random graph\n");
    int option;
    scanf("%d", &option);

    if (option == 1) {
        // Use pre-defined graph
        int predefinedGraph[9][9] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                                      { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                                      { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                                      { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                                      { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                                      { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                                      { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                                      { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                                      { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
        numVertices = 9;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = predefinedGraph[i][j];
            }
        }
    } else if (option == 2) {
        // Enter graph manually
        printf("Enter the number of vertices: ");
        scanf("%d", &numVertices);

        printf("Enter the adjacency matrix:\n");
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                scanf("%d", &graph[i][j]);
            }
        }
    } else if (option == 3) {
        // Generate random graph
        srand(time(NULL)); // Seed random number generator with current time

        printf("Enter the number of vertices: ");
        scanf("%d", &numVertices);

        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (i == j) {
                    graph[i][j] = 0; // No self-loops in the graph
                } else {
                    graph[i][j] = rand() % 10; // Random weight between 0 and 9
                }
            }
        }
    }
    printf("Enter the source vertex: ");
    scanf("%d", &src);

    printf("Enter the destination vertex: ");
    do {
        printf("Enter the destination vertex (must be smaller than %d): ", numVertices);
        scanf("%d", &dest);
    } while (dest >= numVertices);

    // Initialize SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    // Set the blend mode to enable transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    dijkstra(graph, numVertices, src, dest, renderer);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
