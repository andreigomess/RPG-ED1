#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    srand(time(NULL));
    printf("Preparando o mundo...\n");

    // 1. O maestro cria e posiciona todas as estruturas de dados na memória
    EstadoJogo* meuJogo = inicializarJogo();

    // 2. O Game Loop assume o controle e a magia acontece
    iniciarGameLoop(meuJogo);

    // 3. O sistema retorna com sucesso
    destruirJogo(meuJogo);
    return 0;
}