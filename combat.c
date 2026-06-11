#include <stdio.h>
#include "combat.h"

/*
 * Função: iniciarCombate
 * Objetivo: Executa turnos automáticos de ataque até que um personagem morra.
 */
int iniciarCombate(Heroi* heroi, Inimigo inimigo) {
    printf("\n==========================================\n");
    printf("   COMBATE INICIADO! %s vs %s\n", heroi->nome, inimigo.nome);
    printf("==========================================\n\n");

    // O combate continua enquanto ambos tiverem vida positiva
    while (heroi->vidaAtual > 0 && inimigo.vidaAtual > 0) {
        
        // 1. Turno do Herói
        printf("-> %s ataca causando %d de dano!\n", heroi->nome, heroi->poderAtaque);
        inimigo.vidaAtual -= heroi->poderAtaque;

        // Verifica se o inimigo morreu com o golpe
        if (inimigo.vidaAtual <= 0) {
            printf("\n>> %s foi derrotado!\n", inimigo.nome);
            break; // Sai do loop de combate
        }

        // 2. Turno do Inimigo
        printf("-> %s revida causando %d de dano!\n", inimigo.nome, inimigo.poderAtaque);
        heroi->vidaAtual -= inimigo.poderAtaque;

        // Verifica se o herói morreu com o golpe
        if (heroi->vidaAtual <= 0) {
            printf("\n>> O %s sucumbiu aos ferimentos...\n", heroi->nome);
            break; // Sai do loop de combate
        }
    }

    printf("\n==========================================\n");
    printf("           FIM DO COMBATE\n");
    printf("==========================================\n\n");

    // Retorna o resultado para o Game Loop
    if (heroi->vidaAtual > 0) {
        return 1; // Herói sobreviveu (Vitória)
    } else {
        return 0; // Herói morreu (Derrota)
    }
}