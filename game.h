#ifndef GAME_H
#define GAME_H

#include "hero.h"
#include "map.h"
#include "inventory.h"
#include "cards.h"
#include "enemies.h"
#include "scenario.h"
#include "combat.h"

// A super-estrutura que guarda o estado atual de todo o universo do jogo
typedef struct {
    Heroi* heroi;
    MapaCircular* mapa;
    Inventario* mochila;
    PilhaCartas* baralho;
    FilaInimigos* filaInimigos;
    MatrizCenario* cenario;
    int jogoRodando; // 1 = Rodando, 0 = Fim de Jogo (Game Over ou Vitória)
} EstadoJogo;

// Funções de Gerenciamento do Jogo
EstadoJogo* inicializarJogo();
void destruirJogo(EstadoJogo* jogo); // Importante para liberar toda a memória no final!

// O Game Loop e suas 3 fases
void iniciarGameLoop(EstadoJogo* jogo);
void processarInput(EstadoJogo* jogo);
void atualizarLogica(EstadoJogo* jogo);
void renderizarTela(EstadoJogo* jogo);

#endif // GAME_H