#include "game.h"
#include <stdlib.h>

bool initialize_game(Game* game) {
    game->window = NULL;
    game->renderer = NULL;
    game->initialized = false;
    game->running = false;
    game->paused = false;

    const int sdlInitialization = SDL_Init(SDL_INIT_VIDEO);
    if (sdlInitialization != 0) {
        SDL_Log("FATAL ERROR: Cannot initialize SDL.\nError log: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow("SDL TronSnake", 80, 80, 1024, 768, 0);
    if (game->window == NULL) {
        SDL_Log("FATAL ERROR: Cannot create window.\nError log: %s\n", SDL_GetError());
        return false;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        SDL_Log("FATAL ERROR: Cannot create renderer.\nError log: %s\n", SDL_GetError());
        return false;
    }
}

void free_game(Game* game) {
    if (game != NULL) {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        SDL_Quit();
    }
}