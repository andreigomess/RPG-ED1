#ifndef GAME_H
#define GAME_H
#include <string.h>
#include "hero.h"
#include "map.h"
#include "inventory.h"
#include "cards.h"
#include "enemies.h"
#include "scenario.h"
#include "combat.h"

// Definições de Cores ANSI
#define ANSI_RESET   "\x1b[0m"
#define ANSI_VERDE   "\x1b[32m"
#define ANSI_MAGENTA "\x1b[35m" // Vamos usar para o Pântano (Roxo/Magenta)
#define ANSI_AMARELO "\x1b[33m" // Para o Acampamento/Fogueira
#define ANSI_CIANO   "\x1b[36m" // Para o Herói
#define ANSI_BRANCO_BRILHANTE "\x1b[97m"
#define ANSI_CINZA   "\x1b[90m" // Para o Caminho de Terra
#define ANSI_VERMELHO "\x1b[31m" // Para Alertas e Perigo

// Estrutura limpa, sem ponteiros, otimizada para gravação em disco
typedef struct {
    char nome[30];
    int vidaMaxima;
    int vidaAtual;
    int poderAtaque;
    
    Item armaEquipada;
    Item armaduraEquipada;
    Item escudoEquipado;
    Item anelEquipado;

    int voltasCompletas;

    int qtdItensMochila;
    Item itensMochila[9]; // Garante o limite máximo de 9 espaços no arquivo
} DadosSave;

// A super-estrutura que guarda o estado atual de todo o universo do jogo
typedef struct {
    Heroi* heroi;
    MapaCircular* mapa;
    Inventario* mochila;
    PilhaCartas* baralho;
    FilaInimigos* filaInimigos;
    MatrizCenario* cenario;
    int jogoRodando; // 1 = Rodando, 0 = Fim de Jogo (Game Over ou Vitória)
    int voltasCompletas; // Contador de quantas vezes o herói deu a volta completa no mapa
} EstadoJogo;

// Funções de Gerenciamento do Jogo
EstadoJogo* inicializarJogo();
void destruirJogo(EstadoJogo* jogo); // Importante para liberar toda a memória no final!

// O Game Loop e suas 3 fases
void iniciarGameLoop(EstadoJogo* jogo);
void processarInput(EstadoJogo* jogo);
void atualizarLogica(EstadoJogo* jogo);
void renderizarTela(EstadoJogo* jogo);

void salvarJogo(EstadoJogo* jogo);
void carregarJogo(EstadoJogo* jogo);

#endif // GAME_H