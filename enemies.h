#ifndef ENEMIES_H
#define ENEMIES_H

#include <stdlib.h>

/**
 * Estrutura básica de inimigo. Pode ser mantida em listas simples ou usadas
 * por nós da pista (CaminhoNode).
 */
typedef struct Enemy {
    int id;
    char name[48];
    int hp;
    int atk;
    int def;
    struct Enemy *next; /* para listas simples de pooling */
} Enemy;

/* Cria um inimigo com parâmetros básicos. */
Enemy *enemy_create(int id, const char *name, int hp, int atk, int def);

/* Libera um inimigo (e possivelmente sua sub-estrutura). */
void enemy_free(Enemy *e);

/* Função utilitária: gera um inimigo aleatório básico (stub). */
Enemy *enemy_generate_random(void);

#endif /* ENEMIES_H */
