#include "enemies.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

Enemy *enemy_create(int id, const char *name, int hp, int atk, int def) {
    Enemy *e = (Enemy *)malloc(sizeof(Enemy));
    if (!e) return NULL;
    e->id = id;
    strncpy(e->name, name ? name : "Inimigo", sizeof(e->name)-1);
    e->name[sizeof(e->name)-1] = '\0';
    e->hp = hp;
    e->atk = atk;
    e->def = def;
    e->next = NULL;
    return e;
}

void enemy_free(Enemy *e) {
    if (!e) return;
    free(e);
}

Enemy *enemy_generate_random(void) {
    static int seed_initialized = 0;
    if (!seed_initialized) { srand((unsigned)time(NULL)); seed_initialized = 1; }
    int choice = rand() % 3;
    if (choice == 0) return enemy_create(1, "Goblin", 12, 4, 1);
    if (choice == 1) return enemy_create(2, "Lobo", 16, 6, 1);
    return enemy_create(3, "Esqueleto", 10, 5, 0);
}
