#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include "game.h"

/* Forward declaration para evitar dependências circulares */
struct Enemy;

/**
 * Nó da lista circular com sentinela que representa a pista do jogo.
 * Cada nó pode conter informação sobre o terreno e um inimigo opcional.
 */
typedef struct CaminhoNode {
    int id;                     /* identificador do nó */
    int terrain_type;           /* tipo de terreno (plano, floresta, ruínas, etc.) */
    struct Enemy *enemy;        /* ponteiro para inimigo no nó (NULL se vazio) */
    struct CaminhoNode *proximo;
    struct CaminhoNode *anterior;
} CaminhoNode;

/* Cria e retorna um nó sentinela (proximo == anterior == sentinela).
 * A sentinela facilita inserções/remoções e o movimento cíclico.
 */
CaminhoNode *map_create_sentinel(void);

/* Insere `node` imediatamente após `pos` na lista circular. */
void map_insert_after(CaminhoNode *pos, CaminhoNode *node);

/* Remove `node` da lista (não libera o próprio node). */
void map_remove(CaminhoNode *node);

/* Libera toda a lista circular a partir da sentinela (inclui nós e quaisquer
 * objetos referenciados, se aplicável).
 */
void map_destroy(CaminhoNode *sentinel);

/* Inicializa/limpa a matriz estática de entorno (visualização do mapa).
 * A matriz é conceitual e será parte do Game (game->mapa_matrix).
 */
void map_init_matrix(int matrix[MAP_MATRIX_ROWS][MAP_MATRIX_COLS]);

#endif /* MAP_H */
