#include <stdio.h>

#define VIDA_GUERREIRO 500
#define STAMINA_MAX 300
#define VIDA_MAGO 250
#define MANA_MAX 600
#define TAMANHO_MAX_INVENTARIO 4

/*
    Aqui teremos as structs dos personagens:
    - guerreiro e mago são os personagens jogáveis
    - slime e outros são os bonecos que jogaremos contra
*/

struct guerreiro{
    char nome[10] = "Guerreiro";
    int vida = VIDA_GUERREIRO;
    int stamina = STAMINA_MAX;
    int inventario[TAMANHO_MAX_INVENTARIO];
};

struct mago{
    char nome[5] = "Mago";
    int vida = VIDA_MAGO;
    int mana = MANA_MAX;
    int inventario[TAMANHO_MAX_INVENTARIO];
};

struct slime{
    char nome[6] = "Slime";
    int vida = 30;
    int dano = 20;
};

struct bossFinal{
    char nome[5] = "Boss";
    int vida = 1000;
    int dano = 150;
    int danoCrit = 400;
};
