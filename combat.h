#ifndef COMBAT_H
#define COMBAT_H

#include "hero.h"
#include "enemies.h"

// Retorna 1 (Verdadeiro) se o herói venceu, ou 0 (Falso) se o herói foi derrotado
int iniciarCombate(Heroi* heroi, Inimigo inimigo);

#endif // COMBAT_H