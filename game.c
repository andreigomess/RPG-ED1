#include "game.h"
#include "map.h"
#include "hero.h"
#include "inventory.h"
#include "cards.h"
#include "combat.h"
#include "enemies.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Game *game_create(void) {
    Game *g = (Game *)malloc(sizeof(Game));
    if (!g) return NULL;
    memset(g, 0, sizeof(Game));
    g->hero = hero_create("Herói");
    g->map_sentinel = map_create_sentinel();
    g->hero_pos = g->map_sentinel;
    g->card_stack = NULL;
    g->inventory_head = NULL;
    g->combat_queue = queue_create();
    g->enemy_pool = NULL;
    map_init_matrix(g->mapa_matrix);
    g->running = 1;
    return g;
}

void game_destroy(Game *g) {
    if (!g) return;
    if (g->hero) hero_free(g->hero);
    if (g->map_sentinel) map_destroy(g->map_sentinel);
    if (g->card_stack) cards_free(g->card_stack);
    if (g->inventory_head) inventory_free(g->inventory_head);
    if (g->combat_queue) queue_destroy(g->combat_queue);
    /* free enemy pool list */
    Enemy *e = g->enemy_pool;
    while (e) {
        Enemy *nx = e->next;
        enemy_free(e);
        e = nx;
    }
    free(g);
}

void game_input(Game *g) {
    if (!g) return;
    int c = getchar();
    if (c == EOF) return;
    if (c == 'q' || c == 'Q') g->running = 0;
    else if (c == 'i' || c == 'I') {
        /* mostrar inventário simples */
        Item *it = g->inventory_head;
        printf("--- Inventário ---\n");
        while (it) {
            printf("[%d] %s (atk %d def %d)\n", it->id, it->name, it->atk_bonus, it->def_bonus);
            it = it->next;
        }
        printf("------------------\n");
    } else if (c == 'c' || c == 'C') {
        /* usar carta do topo na posição atual do herói */
        if (g->card_stack) {
            Card *top = pop_card(&g->card_stack);
            if (top) {
                printf("Usando carta: %s (efeito %d)\n", top->name, top->effect_type);
                card_apply(g, top, g->hero_pos);
                free(top);
            }
        } else {
            printf("Nenhuma carta disponível.\n");
        }
    }
}

void game_update(Game *g) {
    if (!g) return;
    /* Exemplo: aplicar tick no herói */
    hero_tick(g->hero);
    /* stub: event spawn simples */
    /* Movimento automático do herói: avança um nó por tick */
    if (g->hero_pos && g->hero_pos->proximo) g->hero_pos = g->hero_pos->proximo;

    /* Se a pista está vazia (apenas sentinela), criamos um nó com inimigo */
    if (g->map_sentinel && g->map_sentinel->proximo == g->map_sentinel) {
        CaminhoNode *n = (CaminhoNode *)malloc(sizeof(CaminhoNode));
        n->id = 1;
        n->terrain_type = 0;
        n->enemy = enemy_generate_random();
        n->proximo = n->anterior = NULL;
        map_insert_after(g->map_sentinel, n);
    }

    /* Checar combate: se herói pisou em nó com inimigo -> combate automático */
    if (g->hero_pos && g->hero_pos->enemy) {
        Queue *battle = queue_create();
        if (battle) {
            Combatant *ch = (Combatant *)malloc(sizeof(Combatant));
            Combatant *ce = (Combatant *)malloc(sizeof(Combatant));
            if (ch && ce) {
                ch->kind = COMBATANT_HERO; ch->ptr = g->hero;
                ce->kind = COMBATANT_ENEMY; ce->ptr = g->hero_pos->enemy;
                queue_enqueue(battle, ch);
                queue_enqueue(battle, ce);
                combat_resolve_turns(battle, g);
            } else {
                if (ch) free(ch);
                if (ce) free(ce);
            }
            /* após combate, se inimigo morreu, remover do nó */
            if (g->hero_pos->enemy && g->hero_pos->enemy->hp <= 0) {
                enemy_free(g->hero_pos->enemy);
                g->hero_pos->enemy = NULL;
            }
            queue_destroy(battle);
        }
    }
}

void game_render(Game *g) {
    if (!g) return;
    printf("\n=== Estado do Jogo ===\n");
    if (g->hero) printf("Herói: %s  HP: %d/%d  Nível: %d\n", g->hero->name, g->hero->hp, g->hero->max_hp, g->hero->level);
    /* imprime alguns nós do mapa a partir da sentinela */
    if (g->map_sentinel) {
        CaminhoNode *cur = g->map_sentinel->proximo;
        int count = 0;
        printf("Mapa: ");
        while (cur && cur != g->map_sentinel && count < 10) {
            printf("(id:%d, terr:%d, enem:%s) -> ", cur->id, cur->terrain_type, cur->enemy ? cur->enemy->name : "-");
            cur = cur->proximo;
            count++;
        }
        printf("(sentinela)\n");
    }
    printf("Cartas na pilha: %zu\n", cards_count(g->card_stack));
    printf("======================\n");
}
