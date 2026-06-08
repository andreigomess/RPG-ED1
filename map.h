// Proteção contra múltiplas inclusões do mesmo arquivo
#ifndef MAP_H
#define MAP_H

// Estrutura que representa um "Bloco" ou "Nó" do mapa
typedef struct NoMapa {
    int tipoTerreno;     // Ex: 0 para vazio, 1 para acampamento, etc.
    int idInimigo;       // Ex: 0 para nenhum, 1 para Slime, etc.
    int idEstrutura;     // Modificações aplicadas por cartas
    int idEvento;        // Eventos especiais no bloco
    int linhaVisual;
    int colunaVisual;
    
    // Ponteiro fundamental: aponta para o próximo bloco do caminho
    struct NoMapa* proximo; 
} NoMapa;

// Estrutura principal que gerencia o Mapa Circular
typedef struct {
    NoMapa* sentinela;   // O "marco zero" que controla as voltas do herói
} MapaCircular;

// Declaração da função que vamos usar para criar o mapa pela primeira vez
MapaCircular* inicializarMapa();

// Declaração da função para adicionar um novo bloco ao fim do caminho
void adicionarBloco(MapaCircular* mapa, int tipoTerreno);

// Declaração da função para exibir o mapa no terminal
void imprimirMapa(MapaCircular* mapa);

#endif // MAP_H