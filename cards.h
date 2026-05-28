#ifndef CARDS_H
#define CARDS_H

#include <stdlib.h>

/* forward declarations */
struct Game;
typedef struct CaminhoNode CaminhoNode;

/**
 * Pilha (stack LIFO) de cartas que o jogador usa para modificar o mapa.
 * Implementação baseada em lista encadeada simples com push/pop.
 */
typedef struct Card {
    int id;
    char name[48];
    int effect_type; /* efeito que altera terreno/spawn/etc. */
    struct Card *next; /* topo da pilha */
} Card;

/* Empilha uma carta no topo. */
void push_card(Card **top, Card *c);

/* Remove e retorna a carta do topo. */
Card *pop_card(Card **top);

/* Libera toda a pilha de cartas. */
void cards_free(Card *top);

/* Cria uma carta dinamicamente (fábrica simples). */
Card *card_create(int id, const char *name, int effect_type);

/* Conta quantas cartas estão na pilha. */
size_t cards_count(Card *top);

/* Aplica a carta `c` sobre o nó `node` do mapa usando o estado `g`.
 * Exemplos de efeitos (effect_type):
 * 1 = spawn enemy
 * 2 = remove enemy
 * 3 = toggle terrain (increment)
 */
void card_apply(struct Game *g, Card *c, CaminhoNode *node);

#endif /* CARDS_H */
