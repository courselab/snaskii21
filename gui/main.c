#include <stdio.h>
#include "game.h"

int main() {
    Game game;
    const bool game_created = initialize_game(&game);
    //printf("GAME INITIALIZED: %d\n", game.initialized);
    free_game(&game);
}