#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    // ... (configurações do terminal, se necessário) ...

    // Chama a tela de título
    int escolha = exibirTelaTitulo();

    // Se o usuário desistiu no menu (escolheu 0)
    if (escolha == 0) {
        return 0; 
    }

    // Se chegou aqui, o jogo deve começar
    EstadoJogo* jogo = inicializarJogo(); //

    // Se a escolha foi carregar (1)
    if (escolha == 1) {
        carregarJogo(jogo); // context implies save/load is implemented
    }
    // Se foi Novo Jogo, o 'jogo' já está inicializado como padrão

    // Inicia o motor principal
    iniciarGameLoop(jogo); //

    // Limpa tudo ao fechar
    destruirJogo(jogo); //

    return 0;
}