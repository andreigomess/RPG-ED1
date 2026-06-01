#include <stdio.h>
#include <stdlib.h>
#include "map.h" // Incluímos o cabeçalho que acabamos de criar

/*
 * Função: inicializarMapa
 * Objetivo: Criar a estrutura do mapa e configurar o nó Sentinela.
 */
MapaCircular* inicializarMapa() {
    // 1. Alocamos memória para a estrutura gerenciadora do mapa
    MapaCircular* mapa = (MapaCircular*) malloc(sizeof(MapaCircular));
    
    // Verificação de segurança: checar se a memória foi alocada corretamente
    if (mapa == NULL) {
        printf("Erro de alocação de memória para o mapa!\n");
        exit(1);
    }

    // 2. Alocamos memória para o nó Sentinela
    NoMapa* sentinela = (NoMapa*) malloc(sizeof(NoMapa));
    
    if (sentinela == NULL) {
        printf("Erro de alocação de memória para a sentinela!\n");
        exit(1);
    }

    // 3. Inicializamos os valores do nó Sentinela (ele é vazio por padrão)
    sentinela->tipoTerreno = 0; // Terreno inicial (ex: Acampamento)
    sentinela->idInimigo = 0;
    sentinela->idEstrutura = 0;
    sentinela->idEvento = 0;

    // 4. A MAGIA DA LISTA CIRCULAR: A sentinela aponta para ela mesma!
    // Como não há outros blocos ainda, o "próximo" passo após a sentinela é a própria sentinela.
    sentinela->proximo = sentinela;

    // Conectamos a sentinela ao mapa
    mapa->sentinela = sentinela;

    // Retornamos o mapa pronto para ser usado
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