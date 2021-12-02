#include "snake.h"
#include <stdlib.h>

// Deque functions. Only functions relevant to the game functionality and needs were implemented.
// Deque has it's head at left end and it's tail at right end. 
PositionNode* initialize_node(int positionX, int positionY) {
    PositionNode* node = (PositionNode*) malloc(sizeof(PositionNode));
    if (node == NULL) {
        printf("Error - Unable to allocate memory\n");
        return NULL;
    }

    node->position.x = positionX;
    node->position.y = positionY;
    node->previous = NULL;
    node->next = NULL;
    return node;
}

void free_node(PositionNode** node) {
    if (node == NULL || *node == NULL) {
        return;
    }

    free(*node);
    *node = NULL;
}

Deque* initialize_deque() {
    Deque* deque = (Deque*) malloc(sizeof(Deque));
    if (deque == NULL) {
        printf("Error - Unable to allocate memory\n");
        return NULL;
    }

    deque->head = NULL;
    deque->tail = NULL;
    deque->size = 0;
    return deque;
}

void free_deque(Deque** deque) {
    if (deque == NULL || *deque == NULL) {
        return;
    }

    for (PositionNode* iterator = (*deque)->head; iterator != NULL; ) {
        PositionNode* next = iterator->next;
        free_node(&iterator);
        iterator = next;
    }

    free(*deque);
    *deque = NULL;
}

void deque_push_front(Deque* deque, int positionX, int positionY) {
    if (deque == NULL) {
        return;
    }

    PositionNode* node = initialize_node(positionX, positionY);
    if (deque->head == NULL) {
        deque->head = node;
        deque->tail = node;
    }
    else {
        deque->head->previous = node;
        node->next = deque->head;
        deque->head = node;
    }
    
    ++deque->size;
}

PositionNode* deque_pop_back(Deque* deque) {
    if (deque == NULL) {
        return NULL;
    }

    PositionNode* node = deque->tail;
    deque->tail = deque->tail->previous;
    if (deque->tail == NULL) { // If it's the end of the list, then head = tail = NULL
        deque->head = NULL;
    }
    else {
        deque->tail->next = NULL;
    }

    --deque->size;
    return node;
}

// Snake functions
void initialize_snake(Snake* snake, int headX, int headY, int blockSize) {
    snake->headColor = (SDL_Color) {51, 0, 51, 0}; // Purple
    snake->bodyColor = (SDL_Color) {0, 102, 0, 0}; // Green
    snake->drawShape = (SDL_Rect) {headX, headY, blockSize, blockSize};
    snake->blockSize = blockSize;
    snake->bodyParts = initialize_deque();
    const int initialBodyPartsSize = 4;
    for (int i = initialBodyPartsSize - 1; i >= 0; --i) {
        const int offset = i * blockSize;
        deque_push_front(snake->bodyParts, headX - offset, headY);
    }
    snake->currentDirection = RIGHT;
    snake->incomingDirection = RIGHT;
}

void free_snake(Snake* snake) {
    free_deque(&(snake->bodyParts));
}

void receive_snake_input(Snake* snake, const Uint8* keyboardState) {
    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        snake->incomingDirection = UP;
    }
    else if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        snake->incomingDirection = DOWN;
    }
    else if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        snake->incomingDirection = LEFT;
    }
    else if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        snake->incomingDirection = RIGHT;
    }
}

void update_snake(Snake* snake) {
    PositionNode* newHead = deque_pop_back(snake->bodyParts);

    // If the snake already is moving in the horizontal direction, it's direction can only change to the vertical direction
    if (snake->currentDirection == LEFT || snake->currentDirection == RIGHT) {
        if (snake->incomingDirection == UP) {
            newHead->position.y -= snake->blockSize;
            snake->currentDirection = snake->incomingDirection;
        }
        else if (snake->incomingDirection == DOWN) {
            newHead->position.y += snake->blockSize;
            snake->currentDirection = snake->incomingDirection;
        }
        else {
            newHead->position.x += (snake->currentDirection == LEFT) ? -1 * snake->blockSize : snake->blockSize;
        }
    }
    // If the snake is moving in the vertical direction, it's direction can only change to the horizontal direction
    else if (snake->currentDirection == UP || snake->currentDirection == DOWN) {
        if (snake->incomingDirection == LEFT) {
            newHead->position.x -= snake->blockSize;
            snake->currentDirection = snake->incomingDirection;
        }
        else if (snake->incomingDirection == RIGHT) {
            newHead->position.x += snake->blockSize;
            snake->currentDirection = snake->incomingDirection;
        }
        else {
            newHead->position.y += (snake->currentDirection == UP) ? -1 * snake->blockSize : snake->blockSize;
        }
    }
}

void draw_snake(Snake* snake, SDL_Renderer* renderer) {
    // Draw head
    SDL_SetRenderDrawColor(renderer, snake->headColor.r, snake->headColor.g, snake->headColor.b, snake->headColor.a); 
    PositionNode* snakeHead = snake->bodyParts->head;
    snake->drawShape = (SDL_Rect) {snakeHead->position.x, snakeHead->position.y, snake->blockSize, snake->blockSize};
    SDL_RenderFillRect(renderer, &(snake->drawShape));

    // Draw body
    SDL_SetRenderDrawColor(renderer, snake->bodyColor.r, snake->bodyColor.g, snake->bodyColor.b, snake->bodyColor.a);
    for (PositionNode* iterator = snakeHead->next; iterator != NULL; iterator = iterator->next) {
        snake->drawShape = (SDL_Rect) {iterator->position.x, iterator->position.y, snake->blockSize, snake->blockSize};
        SDL_RenderFillRect(renderer, &(snake->drawShape));
    }
}