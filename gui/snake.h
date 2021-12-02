#ifndef SNAKE_H
#define SNAKE_H

#include "SDL.h"

// Deque related structures
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

// Snake structures
// In SDL, the y-axis is positive downwards and negative upwards
typedef enum {
    UP,
    DOWN,
    LEFT, 
    RIGHT
} Directions;

typedef struct {
    SDL_Color headColor;
    SDL_Color bodyColor;
    SDL_Point headPosition;
    SDL_Rect drawShape;
    int blockSize;
    Deque* bodyParts;
    Directions currentDirection;
    Directions incomingDirection;
} Snake;

void initialize_snake(Snake* snake, int headX, int headY, int blockSize);
void receive_snake_input(Snake* snake, const Uint8* keyboardState);
void draw_snake(Snake* snake, SDL_Renderer* renderer);
void free_snake(Snake* snake);

#endif // SNAKE_H