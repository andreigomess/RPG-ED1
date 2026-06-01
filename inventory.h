#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdlib.h>

/**
 * Item em lista simples encadeada representando o inventário/equipamentos.
 * Lista simples (singly-linked) para inserção/remoção dinâmica.
 */
typedef struct Item {
    int id;
    char name[48];
    int type; /* 0 = consumível, 1 = arma, 2 = armadura, etc. */
    int atk_bonus;
    int def_bonus;
    struct Item *next;
} Item;

/* Adiciona um item ao início da lista (retorna novo head). */
Item *inventory_add(Item *head, Item *it);

/* Remove o primeiro item com `id` e retorna o head atualizado. */
Item *inventory_remove(Item *head, int id);

/* Libera todos os itens da lista. */
void inventory_free(Item *head);

/* Cria um item dinamicamente. Caller deve adicionar com inventory_add
 * Exemplo simples de fábrica de item para drops.
 */
Item *item_create(int id, const char *name, int type, int atk_bonus, int def_bonus);

#endif /* INVENTORY_H */
