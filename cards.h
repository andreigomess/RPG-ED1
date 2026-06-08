#ifndef CARDS_H
#define CARDS_H

// 1. A Estrutura da Carta (Dados do Jogo)
// Baseado nas cartas listadas no final do seu documento
typedef struct {
    char nome[30];
    int idTerreno;      // Qual terreno ela cria (Ex: 3 para Bosque)
    int idInimigoGera;  // Qual inimigo ela pode invocar (Ex: Lobo)
    int curaVida;       // Se for uma carta de recuperação (Ex: Vilarejo)
} Carta;

// 2. A Estrutura do Nó da Pilha
// A "caixa" que guarda a carta e aponta para a carta de baixo
typedef struct NoCarta {
    Carta dados;
    struct NoCarta* abaixo; // Em uma pilha, apontamos para o elemento que está "embaixo"
} NoCarta;

// 3. A Estrutura Gerenciadora da Pilha
typedef struct {
    NoCarta* topo; // O único ponto de acesso de uma pilha é o seu topo!
} PilhaCartas;

// Declarações das funções clássicas de uma Pilha (Stack)
PilhaCartas* inicializarBaralho();

// "Push" -> Coloca uma nova carta no topo do baralho
void empilharCarta(PilhaCartas* pilha, Carta novaCarta);

// "Pop" -> Tira a carta do topo para ser usada (e libera a memória dela)
Carta desempilharCarta(PilhaCartas* pilha);

// "Peek" -> Apenas olha qual é a carta do topo sem tirar ela de lá
void exibirTopo(PilhaCartas* pilha);

#endif // CARDS_H