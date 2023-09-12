#include <iostream>
#include <SDL2/SDL.h>

const int MAX_VERTICES = 9;

int graph[MAX_VERTICES][MAX_VERTICES] = {
    { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
    { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
    { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
    { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
    { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
    { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
    { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
    { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
    { 0, 0, 2, 0, 0, 0, 6, 7, 0 }
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void drawGraph(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            if (graph[i][j] != 0) {
                SDL_RenderDrawLine(renderer, i * 50 + 25, 50, j * 50 + 25, 100);
                // SDL_RenderPresent(renderer);
                SDL_Rect rect_start = {i * 50 + 25, 50, 10, 10};
                // SDL_Rect rect_end = {j * 50 + 25, 100, 10, 10};
                SDL_RenderFillRect(renderer, &rect_start);
                // SDL_RenderFillRect(renderer, &rect_end);
                // SDL_RenderPresent(renderer);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) != 0) {
        std::cout << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_SetWindowTitle(window, "Graph Visualization");
    SDL_Event event;

    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        drawGraph(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
