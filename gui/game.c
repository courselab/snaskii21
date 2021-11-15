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

    game->windowWidth = 1024;
    game->windowHeight = 768;
    game->window = SDL_CreateWindow("SDL TronSnake", 60, 60, game->windowWidth, game->windowHeight, 0);
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

    game->blockSize = 15;
    const int blockSize = game->blockSize;
    Color wallColor = { .red = 178, .green = 208, .blue = 0, .alpha = 255};
    initialize_wall(&game->borders[0], &wallColor, 0, game->windowHeight - blockSize, game->windowWidth, blockSize);
    initialize_wall(&game->borders[1], &wallColor, 0, 0, blockSize, game->windowHeight);
    initialize_wall(&game->borders[2], &wallColor, 0, 0, game->windowWidth, blockSize);
    initialize_wall(&game->borders[3], &wallColor, game->windowWidth - blockSize, 0, blockSize, game->windowHeight);

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
    
    // draw snake and etc here...
    for (int i = 0; i < 4; ++i) {
        Color* wallColor = &(game->borders[i].color);
        SDL_SetRenderDrawColor(game->renderer, wallColor->red, wallColor->green, wallColor->blue, wallColor->alpha);
        SDL_RenderFillRect(game->renderer, &(game->borders[i].shape));
    }
    
    SDL_RenderPresent(game->renderer);
}