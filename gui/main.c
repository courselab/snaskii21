#include <stdio.h>
#include "game.h"

int main() {
    Game game;
    const bool gameCreated = initialize_game(&game);
    if (gameCreated) {
        run_game(&game);
    }
    
    free_game(&game);
}