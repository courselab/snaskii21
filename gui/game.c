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

    game->initialized = true;
    game->running = true;
    game->paused = false;

    return true;
}

void free_game(Game* game) {
    if (game != NULL) {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        SDL_Quit();
    }
}

void run_game(Game* game) {
    if (!game->initialized) {
        printf("FATAL ERROR: Game was not initialized. Please call initialize_game() before running the game.\n");
        return;
    }

    while (game->running) {
        receive_user_input(game);
        update_game(game);
        draw_game(game);
    }
}

void receive_user_input(Game* game) {
    SDL_Event guiEvent;
    while (SDL_PollEvent(&guiEvent)) {
        switch (guiEvent.type) {
            case SDL_QUIT:
            game->running = false;
            break;
            case SDL_KEYDOWN:
                if (guiEvent.key.keysym.sym == SDLK_p) {
                    game->paused = !game->paused;
                }
        }
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        game->running = false;
        game->paused = false;
    }
}

void update_game(Game* game) {
    if (!game->running) {
        return;
    }

    if (!game->paused) {
        printf("RUNNING GAME...\n");
        // add game logic here...
    }
    else {
        printf("GAME PAUSED\n");
    }
}

void draw_game(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    
    // draw snake and walls and etc here...

    SDL_RenderPresent(game->renderer);
}