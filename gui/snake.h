#ifndef SNAKE_H
#define SNAKE_H

#include "SDL.h"

typedef struct {
    SDL_Color headColor;
    SDL_Color bodyColor;
    SDL_Point headPosition;
    SDL_Rect drawShape;
    int size;
    // SDL_Point bodyParts[1000]?
} Snake;

void initialize_snake(Snake* snake, int headX, int headY, int blockSize);

#endif // SNAKE_H