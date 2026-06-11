#ifndef INVENTORY_H
#define INVENTORY_H

// Enumeração (Enum) para facilitar a leitura do tipo de item
// Em vez de usar números soltos, podemos usar a palavra "ARMA", "ARMADURA", etc.
typedef enum {
    ARMA = 1,
    ARMADURA = 2,
    ESCUDO = 3,
    ANEL = 4,
    CONSUMIVEL = 5
} TipoItem;

// 1. Estrutura com os dados reais do equipamento
typedef struct {
    char nome[30];
    TipoItem tipo;
    int valorAtributo; // Ex: +10 de ataque se for arma, +5 de defesa se for armadura
} Item;

// 2. Estrutura do Nó da Lista Simples Encadeada
// Esta é a "caixa" que guarda o item e aponta para o próximo
typedef struct NoItem {
    Item dados;              // A "carga" (o item em si)
    struct NoItem* proximo;  // O ponteiro que liga ao próximo nó da lista
} NoItem;

// 3. Estrutura principal do Inventário
// Diferente da lista circular, aqui só precisamos saber onde a lista começa
typedef struct {
    NoItem* inicio; // Aponta para o primeiro item da mochila
} Inventario;

// Declarações das funções que vamos criar
Inventario* inicializarInventario();
void adicionarItem(Inventario* inv, Item novoItem);
void exibirInventario(Inventario* inv);

// E aqui está a função que resolve sua pergunta sobre liberar memória!
void descartarItem(Inventario* inv, const char* nomeDoItemParaDescartar);

#endif // INVENTORY_H