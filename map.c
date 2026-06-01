#include "map.h"
#include "enemies.h"
#include "game.h"
#include <string.h>

CaminhoNode *map_create_sentinel(void) {
    CaminhoNode *s = (CaminhoNode *)malloc(sizeof(CaminhoNode));
    if (!s) return NULL;
    s->id = 0;
    s->terrain_type = 0;
    s->enemy = NULL;
    s->proximo = s;
    s->anterior = s;
    return s;
}

void map_insert_after(CaminhoNode *pos, CaminhoNode *node) {
    if (!pos || !node) return;
    node->proximo = pos->proximo;
    node->anterior = pos;
    pos->proximo->anterior = node;
    pos->proximo = node;
}

void map_remove(CaminhoNode *node) {
    if (!node) return;
    node->anterior->proximo = node->proximo;
    node->proximo->anterior = node->anterior;
    node->proximo = NULL;
    node->anterior = NULL;
}

void map_destroy(CaminhoNode *sentinel) {
    if (!sentinel) return;
    CaminhoNode *cur = sentinel->proximo;
    while (cur && cur != sentinel) {
        CaminhoNode *next = cur->proximo;
        if (cur->enemy) enemy_free(cur->enemy);
        free(cur);
        cur = next;
    }
    /* free sentinel last */
    free(sentinel);
}

void map_init_matrix(int matrix[MAP_MATRIX_ROWS][MAP_MATRIX_COLS]) {
    for (int r = 0; r < MAP_MATRIX_ROWS; ++r) {
        for (int c = 0; c < MAP_MATRIX_COLS; ++c) {
            matrix[r][c] = 0; /* terreno neutro */
        }
    }
}
