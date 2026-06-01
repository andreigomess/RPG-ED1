#include "combat.h"
#include "game.h"
#include "hero.h"
#include "enemies.h"
#include "inventory.h"
#include "cards.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Queue *queue_create(void) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q) return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void queue_enqueue(Queue *q, Combatant *combatant) {
    if (!q || !combatant) return;
    TurnNode *n = (TurnNode *)malloc(sizeof(TurnNode));
    if (!n) return;
    n->combatant = combatant;
    if (q->size == 0) {
        n->next = n;
        q->head = n;
        q->tail = n;
    } else {
        n->next = q->head;
        q->tail->next = n;
        q->tail = n;
    }
    q->size++;
}

Combatant *queue_dequeue(Queue *q) {
    if (!q || q->size == 0) return NULL;
    TurnNode *n = q->head;
    Combatant *c = n->combatant;
    if (q->size == 1) {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = n->next;
        q->tail->next = q->head;
    }
    free(n);
    q->size--;
    return c;
}

void queue_destroy(Queue *q) {
    if (!q) return;
    while (q->size > 0) {
        Combatant *c = queue_dequeue(q);
        if (c) free(c);
    }
    free(q);
}

static int calc_damage(int atk, int def) {
    int dmg = atk - def;
    if (dmg < 1) dmg = 1;
    return dmg;
}

void combat_resolve_turns(Queue *q, struct Game *g) {
    if (!q || !g) return;

    /* Encontra o herói na fila (deve existir) */
    Hero *h = g->hero;
    if (!h) return;

    printf("Iniciando combate múltiplo com %zu participantes\n", q->size);

    /* Loop até que herói morra ou não haja inimigos vivos */
    while (h->hp > 0) {
        /* conta inimigos vivos */
        int enemies_alive = 0;
        TurnNode *tn = q->head;
        for (size_t i = 0; i < q->size; ++i) {
            if (tn && tn->combatant && tn->combatant->kind == COMBATANT_ENEMY) {
                Enemy *ee = (Enemy *)tn->combatant->ptr;
                if (ee && ee->hp > 0) enemies_alive++;
            }
            tn = tn ? tn->next : NULL;
        }
        if (enemies_alive == 0) break; /* vitória do herói */

        /* Processa uma volta completa da fila atual */
        size_t rounds = q->size;
        for (size_t i = 0; i < rounds; ++i) {
            Combatant *act = queue_dequeue(q);
            if (!act) continue;

            if (act->kind == COMBATANT_HERO) {
                if (h->hp <= 0) { free(act); continue; }
                /* herói ataca o primeiro inimigo vivo na fila */
                TurnNode *scan = q->head;
                Enemy *target = NULL;
                for (size_t j = 0; j < q->size; ++j) {
                    if (scan && scan->combatant && scan->combatant->kind == COMBATANT_ENEMY) {
                        Enemy *cand = (Enemy *)scan->combatant->ptr;
                        if (cand && cand->hp > 0) { target = cand; break; }
                    }
                    scan = scan ? scan->next : NULL;
                }
                if (target) {
                    int dmg = calc_damage(h->atk, target->def);
                    target->hp -= dmg;
                    printf("%s ataca %s por %d (hp %d)\n", h->name, target->name, dmg, target->hp);
                    if (target->hp <= 0) {
                        printf("%s derrotado!\n", target->name);
                        /* drop */
                        Item *it = item_create(100 + target->id, "Drop Genérico", 0, 1, 0);
                        if (it) g->inventory_head = inventory_add(g->inventory_head, it);
                        Card *c = card_create(200 + target->id, "Carta Drop", 1);
                        if (c) push_card(&g->card_stack, c);
                    }
                }
                /* re-enfileira o herói se ainda vivo */
                if (h->hp > 0) queue_enqueue(q, act);
                else free(act);
            } else if (act->kind == COMBATANT_ENEMY) {
                Enemy *ee = (Enemy *)act->ptr;
                if (!ee || ee->hp <= 0) { free(act); continue; }
                /* inimigo ataca herói */
                int dmg = calc_damage(ee->atk, h->def);
                h->hp -= dmg;
                printf("%s ataca %s por %d (hp %d)\n", ee->name, h->name, dmg, h->hp);
                /* se inimigo ainda vivo, re-enfileira */
                if (ee->hp > 0) queue_enqueue(q, act);
                else {
                    /* enemy morreu por outro efeito possivelmente; criar drops */
                    Item *it = item_create(100 + ee->id, "Drop Genérico", 0, 1, 0);
                    if (it) g->inventory_head = inventory_add(g->inventory_head, it);
                    Card *c = card_create(200 + ee->id, "Carta Drop", 1);
                    if (c) push_card(&g->card_stack, c);
                    free(act);
                }
            } else {
                free(act);
            }

            if (h->hp <= 0) break;
        }
        /* pequena pausa conceitual entre rodadas (poderia ser tick) */
    }

    if (h->hp <= 0) {
        printf("%s morreu. GAME OVER.\n", h->name);
        g->running = 0;
    }
}

