#include "snake.h"

void initialize_snake(Snake* snake, int headX, int headY, int blockSize) {
    snake->headColor = (SDL_Color) {51, 0, 51, 0};
    snake->bodyColor = (SDL_Color) {0, 102, 0, 0};
    snake->headPosition = (SDL_Point) {headX, headY};
    snake->drawShape = (SDL_Rect) {headX, headY, blockSize, blockSize};
    snake->size = blockSize;
}

void draw_snake(Snake* snake, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, snake->headColor.r, snake->headColor.g, snake->headColor.b, snake->headColor.a); 
    SDL_RenderFillRect(renderer, &(snake->drawShape));
}