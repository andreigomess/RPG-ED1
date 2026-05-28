#ifndef HERO_H
#define HERO_H

#include <stdint.h>

/* O herói possui atributos básicos e ponteiros para equipamentos/inventário
 * (representados como lista simples de `Item`, definida em `inventory.h`).
 */
typedef struct Hero {
    char name[32];
    int level;
    int exp;
    int hp;
    int max_hp;
    int atk;
    int def;
    /* Ponteiros para estruturas externas (forward declarations em tempo de uso) */
    void *equipped_head; /* ponteiro genérico para lista de equipamentos (Item*) */
} Hero;

/* Cria e inicializa um herói padrão. */
Hero *hero_create(const char *name);

/* Desaloca o herói e quaisquer estruturas associadas. */
void hero_free(Hero *h);

/* Processa efeitos passivos do herói por tick (ex: regeneração). */
void hero_tick(Hero *h);

#endif /* HERO_H */
