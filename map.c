#include <stdio.h>
#include <stdlib.h>
#include "map.h" // Incluímos o cabeçalho que acabamos de criar

/*
 * Função: inicializarMapa
 * Objetivo: Criar a estrutura do mapa e configurar o nó Sentinela.
 */
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

/*
 * Função: inicializarMapa
 * Objetivo: Construir uma Lista Circular de 16 nós sincronizada com o desenho da matriz.
 */
MapaCircular* inicializarMapa() {
    MapaCircular* mapa = (MapaCircular*) malloc(sizeof(MapaCircular));
    if (mapa == NULL) {
        printf("Erro crítico: Falha ao alocar o gerenciador do mapa!\n");
        exit(1);
    }

    // Coordenadas exatas que formam um circuito quadrado oco (sentido horário)
    // Começa em (1,1), vai até (1,5), desce até (5,5), volta até (5,1) e sobe até (2,1)
    int linhasPista[16]  = {1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2};
    int colunasPista[16] = {1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 1, 1, 1};

    NoMapa* primeiroNo = NULL;
    NoMapa* noAnterior = NULL;

    // Criamos exatamente os 16 nós da nossa pista
    for (int i = 0; i < 16; i++) {
        NoMapa* novoNo = (NoMapa*) malloc(sizeof(NoMapa));
        if (novoNo == NULL) {
            printf("Erro ao alocar o bloco %d do mapa!\n", i);
            exit(1);
        }

        // Inicializamos os dados padrão do bloco
        novoNo->tipoTerreno = 1;  // 1 = Caminho padrão
        novoNo->idInimigo = 0;    // Começa sem monstros fixos
        novoNo->idEstrutura = 0;  // Sem cartas jogadas ainda
        novoNo->idEvento = 0;

        // Injetamos o "GPS" visual sincronizado com a matriz!
        novoNo->linhaVisual = linhasPista[i];
        novoNo->colunaVisual = colunasPista[i];

        // Encadeamento lógico da lista
        if (primeiroNo == NULL) {
            primeiroNo = novoNo; // Guarda o início para fechar o círculo depois
        } else {
            noAnterior->proximo = novoNo; // O anterior aponta para o novo
        }
        noAnterior = novoNo;
    }

    // A grande magia da Lista Circular: o 16º nó aponta de volta para o 1º!
    noAnterior->proximo = primeiroNo;

    // O nosso gerenciador guarda o primeiro nó como o "marco zero" (Sentinela)
    mapa->sentinela = primeiroNo;

    return mapa;
}

/*
 * Função: adicionarBloco
 * Objetivo: Inserir um novo espaço no mapa, no final do caminho, mantendo o formato circular.
 */
void adicionarBloco(MapaCircular* mapa, int tipoTerreno) {
    // 1. Criamos o novo bloco e alocamos memória
    NoMapa* novoBloco = (NoMapa*) malloc(sizeof(NoMapa));
    
    // Verificação de segurança
    if (novoBloco == NULL) {
        printf("Erro ao alocar memoria para novo bloco do mapa!\n");
        return; // Sai da função em caso de erro
    }

    // 2. Preenchemos os dados básicos do bloco
    novoBloco->tipoTerreno = tipoTerreno;
    novoBloco->idInimigo = 0;   // Começa sem inimigos
    novoBloco->idEstrutura = 0; // Começa sem estruturas
    novoBloco->idEvento = 0;    // Começa sem eventos especiais

    // 3. Encontramos o último bloco atual da pista
    // Começamos nossa busca a partir da sentinela
    NoMapa* atual = mapa->sentinela;
    
    // Enquanto o próximo bloco não for a sentinela, avançamos para o próximo.
    // Quando o laço parar, a variável 'atual' estará guardando o último bloco da pista.
    while (atual->proximo != mapa->sentinela) {
        atual = atual->proximo;
    }

    // 4. Inserimos o novo bloco e fechamos o ciclo
    atual->proximo = novoBloco;           // O antigo último agora aponta para o novo bloco
    novoBloco->proximo = mapa->sentinela; // O novo bloco aponta para a sentinela (fecha o ciclo)
}

/*
 * Função: imprimirMapa
 * Objetivo: Percorrer a lista circular e exibir os blocos no terminal.
 */
void imprimirMapa(MapaCircular* mapa) {
    // 1. Verificação de segurança: O mapa está vazio?
    // Se a sentinela aponta para ela mesma, não há blocos no caminho.
    if (mapa->sentinela->proximo == mapa->sentinela) {
        printf("O mapa esta vazio! Apenas o vazio das memorias apagadas existe aqui.\n");
        return; // Encerra a função cedo
    }

    printf("\n=== MAPA DO HERÓI ===\n");
    printf("[Sentinela] -> \n");
    
    // 2. Ponto de partida
    // Começamos do primeiro bloco real, que está logo após a sentinela.
    NoMapa* atual = mapa->sentinela->proximo;
    int contadorBloco = 1; // Uma variável simples para enumerarmos os passos

    // 3. A Caminhada (Loop)
    // O laço continua rodando enquanto não voltarmos para a sentinela
    while (atual != mapa->sentinela) {
        // Imprime as informações do bloco atual
        printf("  Passo %d: Terreno tipo %d\n", contadorBloco, atual->tipoTerreno);
        
        // Regra de ouro das listas encadeadas: sempre avançar o ponteiro!
        atual = atual->proximo;
        contadorBloco++;
    }
    
    // 4. Conclusão visual do ciclo
    printf("-> [Volta para a Sentinela]\n");
    printf("=====================\n\n");
}