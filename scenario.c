#include <stdio.h>
#include <stdlib.h>
#include "scenario.h"

/*
 * Função: inicializarCenario
 * Objetivo: Criar a matriz e preencher tudo com '0' (Vazio).
 */
MatrizCenario* inicializarCenario() {
    MatrizCenario* cenario = (MatrizCenario*) malloc(sizeof(MatrizCenario));
    
    if (cenario == NULL) {
        printf("Erro ao alocar memoria para o cenario!\n");
        exit(1);
    }
    
    // Varre todas as linhas e colunas para zerar o mapa inicial
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            cenario->grade[i][j] = 0; // 0 significa terreno vazio/apagado
        }
    }

    for (int i = 1; i <= 5; i++) {
        cenario->grade[1][i] = 1; // Borda Superior
        cenario->grade[5][i] = 1; // Borda Inferior
        cenario->grade[i][1] = 1; // Borda Esquerda
        cenario->grade[i][5] = 1; // Borda Direita
    }
    
    return cenario;
}

/*
 * Função: colocarEstrutura
 * Objetivo: Modificar um bloco da matriz com base nas cartas usadas (Ex: Campo, Mansão).
 */
void colocarEstrutura(MatrizCenario* cenario, int linha, int coluna, int idEstrutura) {
    // Verificação de segurança para não tentar escrever fora da matriz
    if (linha >= 0 && linha < LINHAS && coluna >= 0 && coluna < COLUNAS) {
        cenario->grade[linha][coluna] = idEstrutura;
    } else {
        printf("Posição invalida no cenario!\n");
    }
}

/*
 * Função: exibirCenario
 * Objetivo: Desenhar o grid no terminal de forma visual.
 */
void exibirCenario(MatrizCenario* cenario, int linhaHeroi, int colunaHeroi) {
    printf("\n=== MUNDO AO REDOR ===\n");
    
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            if (i == linhaHeroi && j == colunaHeroi) {
                printf("[H] "); // H de Herói!
                continue; // Pula para a próxima coluna, ignorando o resto abaixo
            }
            
            int blocoAtual = cenario->grade[i][j];
            
            // Arte ASCII simples traduzindo os números
            if (blocoAtual == 0) {
                printf("[ ] "); // Vazio
            } else if (blocoAtual == 1) {
                printf("[C] "); // Reservado para o Caminho Circular do Herói no futuro
            } else if (blocoAtual == 4) {
                printf("[V] "); // Ex: 4 = Mansão Vampírica
            } else {
                printf("[%d] ", blocoAtual); // Outros tipos genéricos
            }
        }
        // Ao final de cada linha da matriz, quebramos a linha no terminal
        printf("\n");
    }
    printf("======================\n\n");
}