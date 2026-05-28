#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(void) {
    Game *g = game_create();
    if (!g) {
        fprintf(stderr, "Falha ao inicializar jogo\n");
        return 1;
    }

    printf("Herói em Loop - implementação inicial\n");
    printf("Comandos: 'q' quit, 'i' inventário (press Enter)\n");

    while (g->running) {
        game_input(g);
        game_update(g);
        game_render(g);
    }

    printf("Encerrando jogo. Liberando recursos...\n");
    game_destroy(g);
    return 0;
}

