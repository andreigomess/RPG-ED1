#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hero.h"

/*
 * Função: inicializarHeroi
 * Objetivo: Criar o herói e colocá-lo no início do mapa.
 */
Heroi* inicializarHeroi(MapaCircular* mapa) {
    Heroi* novoHeroi = (Heroi*) malloc(sizeof(Heroi));
    
    if (novoHeroi == NULL) {
        printf("Erro ao alocar memoria para o heroi!\n");
        exit(1);
    }

    strcpy(novoHeroi->nome, "Viajante Esquecido");
    novoHeroi->vidaMaxima = 100;
    novoHeroi->vidaAtual = 100;

    // Colocamos o herói no primeiro bloco de verdade (logo após a sentinela)
    novoHeroi->posicaoAtual = mapa->sentinela->proximo;

    return novoHeroi;
}

/*
 * Função: moverHeroi
 * Objetivo: Avançar o ponteiro do herói para o próximo bloco.
 */
void moverHeroi(Heroi* heroi, MapaCircular* mapa) {
    // O herói dá um passo para a frente
    heroi->posicaoAtual = heroi->posicaoAtual->proximo;

    // Se o próximo passo for a Sentinela, ele deve pular, 
    // pois a Sentinela é apenas o marco de controle do loop.
    if (heroi->posicaoAtual == mapa->sentinela) {
        heroi->posicaoAtual = heroi->posicaoAtual->proximo;
    }
}

/*
 * Função: traduzirTerreno
 * Objetivo: Converter o número do terreno em uma palavra amigável.
 * Nota: Esta função é "auxiliar" e usada apenas dentro deste arquivo.
 */
const char* traduzirTerreno(int tipo) {
    switch(tipo) {
        case 1: return "Acampamento";
        case 2: return "Caminho de Terra";
        case 3: return "Bosque Sombrio";
        default: return "Vazio";
    }
}

/*
 * Função: desenharMapaComHeroi
 * Objetivo: Percorrer a pista e desenhar uma arte ASCII simples.
 */
void desenharMapaComHeroi(Heroi* heroi, MapaCircular* mapa) {
    // Limpa a tela do terminal (funciona na maioria dos sistemas)
    // No Windows use "cls", no Linux/Mac use "clear"
    system("clear || cls"); 

    printf("\n====================================================\n");
    printf(" Herói: %s | HP: %d/%d\n", heroi->nome, heroi->vidaAtual, heroi->vidaMaxima);
    printf("====================================================\n\n");

    NoMapa* atual = mapa->sentinela->proximo;

    printf("Loop: ");

    // 1ª Parte: Desenhar a linha visual do mapa
    while (atual != mapa->sentinela) {
        // Se a posição que estamos desenhando for a posição do herói, desenhamos [H]
        if (atual == heroi->posicaoAtual) {
            printf("[H]-> ");
        } else {
            // Caso contrário, desenhamos o terreno
            if (atual->tipoTerreno == 1) printf("[A]-> "); // Acampamento
            else if (atual->tipoTerreno == 2) printf("[C]-> "); // Caminho
            else if (atual->tipoTerreno == 3) printf("[B]-> "); // Bosque
            else printf("[ ]-> ");
        }
        atual = atual->proximo;
    }
    printf("(Volta ao inicio)\n\n");

    // 2ª Parte: Dizer onde o herói está em texto
    printf("Você está pisando em: ** %s **\n", traduzirTerreno(heroi->posicaoAtual->tipoTerreno));
    printf("\n====================================================\n");
}