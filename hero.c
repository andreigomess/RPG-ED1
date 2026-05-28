#include "hero.h"
#include <string.h>
#include <stdlib.h>

Hero *hero_create(const char *name) {
    Hero *h = (Hero *)malloc(sizeof(Hero));
    if (!h) return NULL;
    strncpy(h->name, name ? name : "Herói", sizeof(h->name)-1);
    h->name[sizeof(h->name)-1] = '\0';
    h->level = 1;
    h->exp = 0;
    h->max_hp = 100;
    h->hp = h->max_hp;
    h->atk = 10;
    h->def = 2;
    h->equipped_head = NULL;
    return h;
}

void hero_free(Hero *h) {
    if (!h) return;
    /* Equipamentos/inventário são gerenciados separadamente; apenas free aqui */
    free(h);
}

void hero_tick(Hero *h) {
    if (!h) return;
    /* Exemplo simples: regeneração passiva pequena */
    if (h->hp < h->max_hp) h->hp += 1;
    if (h->hp > h->max_hp) h->hp = h->max_hp;
}
