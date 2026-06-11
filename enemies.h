#ifndef ENEMIES_H
#define ENEMIES_H

// 1. A Estrutura do Inimigo (Dados do Jogo)
// O sistema de combate precisa de atributos para calcular o dano e a vida [cite: 536]
typedef struct {
    char nome[30];
    int vidaMaxima;
    int vidaAtual;
    int poderAtaque;
} Inimigo;

// 2. A Estrutura do Nó da Fila
// A "caixa" que guarda o inimigo na fila de espera e aponta para o de trás
typedef struct NoFila {
    Inimigo dados;
    struct NoFila* proximo; 
} NoFila;

// 3. A Estrutura Gerenciadora da Fila (FIFO)
// Controla a ordem de inimigos e eventos com eficiência [cite: 159, 160]
typedef struct {
    NoFila* inicio; // De onde tiramos o inimigo para colocá-lo no mapa (Dequeue)
    NoFila* fim;    // Onde inserimos o novo inimigo que acabou de ser gerado (Enqueue)
} FilaInimigos;

// Declarações das funções da Fila Dinâmica
FilaInimigos* inicializarFila();

// "Enqueue" -> Coloca um inimigo no final da fila de espera
void enfileirarInimigo(FilaInimigos* fila, Inimigo novoInimigo);

// "Dequeue" -> Tira o primeiro inimigo da fila para processá-lo e colocá-lo no jogo
Inimigo desenfileirarInimigo(FilaInimigos* fila);

// Exibe quem está esperando na fila (útil para testes e depuração)
void exibirFila(FilaInimigos* fila);

#endif // ENEMIES_H