#ifndef GAME_H
#define GAME_H

#include <stdint.h>

/* Tamanhos da matriz estática que representa o entorno da pista */
#define MAP_MATRIX_ROWS 5
#define MAP_MATRIX_COLS 5

/* Encaminhamentos para estruturas definidas em outros módulos */
struct Hero;
struct CaminhoNode;
struct Card;
struct Item;
struct Queue;
struct Enemy;

/**
 * Estrutura central que reúne o estado global do jogo.
 * Contém ponteiros para as principais estruturas (herói, mapa, pilha de cartas,
 * inventário, fila de combate, etc.).
 */
typedef struct Game {
    struct Hero *hero;
    struct CaminhoNode *map_sentinel; /* lista circular com sentinela */
    struct CaminhoNode *hero_pos;     /* posição atual do herói na pista */
    struct Card *card_stack;          /* pilha LIFO */
    struct Item *inventory_head;      /* lista encadeada simples */
    struct Queue *combat_queue;       /* fila circular para turnos */
    struct Enemy *enemy_pool;         /* lista/simple apontador para gerados */
    int mapa_matrix[MAP_MATRIX_ROWS][MAP_MATRIX_COLS]; /* representação 2D estática */
    int running; /* flag para while(jogo_rodando) */
} Game;

/* Inicializa o estado mínimo do jogo (aloca estruturas se necessário).
 * Deve ser chamada antes de entrar no loop principal.
 */
Game *game_create(void);

/* Libera todos os recursos associados ao `Game` de forma segura.
 * Implementar para evitar memory leaks ao encerrar o jogo.
 */
void game_destroy(Game *g);

/* Funções de ciclo, serão implementadas posteriormente:
 * - game_input: processa entrada do jogador
 * - game_update: atualiza estado (movimento, spawns, combate automático)
 * - game_render: imprime estado no terminal
 */
void game_input(Game *g);
void game_update(Game *g);
void game_render(Game *g);

#endif /* GAME_H */
