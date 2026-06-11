#ifndef HERO_H
#define HERO_H

#include "inventory.h"

#include "map.h" // Precisamos incluir o mapa para usar o NoMapa

// Estrutura que representa o jogador
typedef struct {
    char nome[30];
    int vidaMaxima;
    int vidaAtual;
    int poderAtaque;

    Item armaEquipada;
    Item armaduraEquipada;
    Item escudoEquipado;
    Item anelEquipado;
    
    // Ponteiro crucial: guarda a posição exata do herói na Lista Circular
    NoMapa* posicaoAtual; 
} Heroi;

// Declarações das funções
Heroi* inicializarHeroi(MapaCircular* mapa);
void moverHeroi(Heroi* heroi, MapaCircular* mapa);
void desenharMapaComHeroi(Heroi* heroi, MapaCircular* mapa);

#endif // HERO_H