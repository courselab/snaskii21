#include "game.h"
#include <stdlib.h>
#include <stdio.h>

bool initialize_game(Game* game) {
    game->window = NULL;
    game->renderer = NULL;
    game->initialized = false;
    game->running = false;
    game->paused = false;
    game->score = 0;

    const int sdlInitialization = SDL_Init(SDL_INIT_VIDEO);
    if (sdlInitialization != 0) {
        SDL_Log("FATAL ERROR: Cannot initialize SDL.\nError log: %s\n", SDL_GetError());
        return false;
    }

    TTF_Init();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    game->font = TTF_OpenFont("Road_Rage/RoadRage-Regular.ttf", 20);
    if (game->font == NULL) {
        SDL_Log("FATAL ERROR: Cannot load font.\n");
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

    game->blockSize = 16;
    const int blockSize = game->blockSize;
    Color wallColor = { .red = 178, .green = 208, .blue = 0, .alpha = 255};
    initialize_wall(&game->borders[0], &wallColor, 0, game->windowHeight - blockSize, game->windowWidth, blockSize);
    initialize_wall(&game->borders[1], &wallColor, 0, 0, blockSize, game->windowHeight);
    initialize_wall(&game->borders[2], &wallColor, 0, 0, game->windowWidth, blockSize);
    initialize_wall(&game->borders[3], &wallColor, game->windowWidth - blockSize, 0, blockSize, game->windowHeight);
    initialize_snake(&(game->snake), game->windowWidth / 2, game->windowHeight / 2, game->blockSize);
    
    return true;
}

void free_game(Game* game) {
    if (game != NULL) {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        TTF_Quit();
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
}

void draw_game(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    
    draw_walls(game);
    draw_text(game);

    SDL_RenderPresent(game->renderer);
}

void draw_walls(Game* game) {
    for (int i = 0; i < 4; ++i) {
        Color* wallColor = &(game->borders[i].color);
        SDL_SetRenderDrawColor(game->renderer, wallColor->red, wallColor->green, wallColor->blue, wallColor->alpha);
        SDL_RenderFillRect(game->renderer, &(game->borders[i].shape));
    }
}

void draw_text(Game* game) {
    SDL_Color white = {255, 255, 255};
    char scoreMessage[64];
    snprintf(scoreMessage, 12, "Score : %d", game->score);
    game->textSurface = TTF_RenderText_Solid(game->font, scoreMessage, white);
    game->textTexture = SDL_CreateTextureFromSurface(game->renderer, game->textSurface);
    SDL_Rect textPosition = {2*game->blockSize, 2*game->blockSize, 6*game->blockSize, 4*game->blockSize};
    SDL_RenderCopy(game->renderer, game->textTexture, NULL, &textPosition);
    
    if (game->paused) {
        game->textSurface = TTF_RenderText_Solid(game->font, "PAUSED", white);
        game->textTexture = SDL_CreateTextureFromSurface(game->renderer, game->textSurface);
        textPosition = (SDL_Rect) {game->windowWidth / 3, game->windowHeight / 3, 20*game->blockSize, 12*game->blockSize};
        SDL_RenderCopy(game->renderer, game->textTexture, NULL, &textPosition);
    }
    
    SDL_DestroyTexture(game->textTexture);
    SDL_FreeSurface(game->textSurface);
}