#ifndef SNAKE_H
#define SNAKE_H

#include "SDL.h"

struct node {
    SDL_Point position;
    struct node* next;
    struct node* previous;
};

struct deque {
    struct node* head;
    struct node* tail;
    int size;
};

typedef struct node PositionNode;
typedef struct deque Deque;

typedef struct {
    SDL_Color headColor;
    SDL_Color bodyColor;
    SDL_Point headPosition;
    SDL_Rect drawShape;
    int blockSize;
    Deque* bodyParts;
} Snake;

void initialize_snake(Snake* snake, int headX, int headY, int blockSize);
void draw_snake(Snake* snake, SDL_Renderer* renderer);
void free_snake(Snake* snake);

#endif // SNAKE_H