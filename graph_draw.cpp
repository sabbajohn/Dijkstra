#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void drawGraph(SDL_Renderer* renderer)
{
    // Background color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Graph edges
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 100, 200, 300, 200);
    SDL_RenderDrawLine(renderer, 200, 100, 200, 300);

    // Graph vertices
    SDL_Rect vertex1 = { 90, 190, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &vertex1);

    SDL_Rect vertex2 = { 290, 190, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &vertex2);

    // Update the screen
    SDL_RenderPresent(renderer);
}

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Graph Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event event;

    // Main loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            // Quit event
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Draw the graph
        drawGraph(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
