#pragma once

#include "color.h"
#include "SDL.h"

typedef struct {
    Color color;
    SDL_Rect shape;
} Wall;

void initialize_wall(Wall* wall, Color* color, float topLeftX, float topLeftY, float width, float height);