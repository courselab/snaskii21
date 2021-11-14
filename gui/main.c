#include <stdio.h>
#include "game.h"

int main() {
    Game game;
    const bool gameCreated = initialize_game(&game);
    if (gameCreated) {
        printf("GAME\n");
    }
    
    free_game(&game);
}