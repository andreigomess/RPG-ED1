#ifndef COMBAT_H
#define COMBAT_H

#include <stdlib.h>

/* Tipo de combatant usado na fila para distinguir Hero/Enemy. */
typedef enum {
    COMBATANT_HERO = 0,
    COMBATANT_ENEMY = 1
} CombatantKind;

typedef struct Combatant {
    CombatantKind kind;
    void *ptr; /* Hero* ou Enemy* */
} Combatant;

/* Nó da fila contém um ponteiro para Combatant. */
typedef struct TurnNode {
    Combatant *combatant;
    struct TurnNode *next;
} TurnNode;

typedef struct Queue {
    TurnNode *head;
    TurnNode *tail;
    size_t size;
} Queue;

Queue *queue_create(void);
void queue_enqueue(Queue *q, Combatant *combatant);
Combatant *queue_dequeue(Queue *q);
void queue_destroy(Queue *q);

/* Resolve um combate usando a fila de turnos. Recebe um ponteiro para o
 * Game para poder aplicar drops e mudar estado global (ex: acabar o jogo).
 */
struct Game;
void combat_resolve_turns(Queue *q, struct Game *g);

#endif /* COMBAT_H */
