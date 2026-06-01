#include "cards.h"
#include "map.h"
#include "enemies.h"
#include <stdlib.h>
#include <string.h>

void push_card(Card **top, Card *c) {
    if (!top || !c) return;
    c->next = *top;
    *top = c;
}

Card *pop_card(Card **top) {
    if (!top || !*top) return NULL;
    Card *ret = *top;
    *top = ret->next;
    ret->next = NULL;
    return ret;
}

void cards_free(Card *top) {
    Card *cur = top;
    while (cur) {
        Card *next = cur->next;
        free(cur);
        cur = next;
    }
}

Card *card_create(int id, const char *name, int effect_type) {
    Card *c = (Card *)malloc(sizeof(Card));
    if (!c) return NULL;
    c->id = id;
    strncpy(c->name, name ? name : "Carta", sizeof(c->name)-1);
    c->name[sizeof(c->name)-1] = '\0';
    c->effect_type = effect_type;
    c->next = NULL;
    return c;
}

size_t cards_count(Card *top) {
    size_t n = 0;
    Card *cur = top;
    while (cur) { n++; cur = cur->next; }
    return n;
}

void card_apply(struct Game *g, Card *c, struct CaminhoNode *node) {
    if (!g || !c || !node) return;
    switch (c->effect_type) {
        case 1: /* spawn enemy */
            if (!node->enemy) node->enemy = enemy_generate_random();
            break;
        case 2: /* remove enemy */
            if (node->enemy) {
                enemy_free(node->enemy);
                node->enemy = NULL;
            }
            break;
        case 3: /* toggle terrain */
            node->terrain_type = (node->terrain_type + 1) % 3;
            break;
        default:
            break;
    }
}
