#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cards.h"

/*
 * Função: inicializarBaralho
 * Objetivo: Criar a mão vazia do jogador para começar a receber as cartas.
 */
PilhaCartas* inicializarBaralho() {
    PilhaCartas* pilha = (PilhaCartas*) malloc(sizeof(PilhaCartas));
    
    if (pilha == NULL) {
        printf("Erro de alocação de memória para o baralho!\n");
        exit(1);
    }
    
    // A pilha começa vazia, então não há carta no topo
    pilha->topo = NULL; 
    return pilha;
}

/*
 * Função: empilharCarta (O Push)
 * Objetivo: Colocar uma nova carta adquirida no topo da pilha.
 */
void empilharCarta(PilhaCartas* pilha, Carta novaCarta) {
    // 1. Criamos a "caixa" que vai guardar a carta
    NoCarta* novoNo = (NoCarta*) malloc(sizeof(NoCarta));
    
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria para a nova carta!\n");
        return;
    }
    
    // 2. Colocamos os dados dentro do nó
    novoNo->dados = novaCarta;
    
    // 3. A mágica do empilhamento:
    // A nova carta aponta para quem estava no topo antes dela
    novoNo->abaixo = pilha->topo;
    
    // Agora, o topo oficial da pilha passa a ser a nossa nova carta
    pilha->topo = novoNo;
    
    printf(">> Você comprou a carta '%s'. Ela esta no topo do baralho!\n", novaCarta.nome);
}

/*
 * Função: desempilharCarta (O Pop)
 * Objetivo: Pegar a carta do topo para usá-la no mapa e limpar a memória dela.
 */
Carta desempilharCarta(PilhaCartas* pilha) {
    // Carta de segurança para caso de erro (quando tentar puxar de um baralho vazio)
    Carta cartaVazia = {"Vazia", 0, 0, 0}; 
    
    // 1. Verificamos se há algo para puxar
    if (pilha->topo == NULL) {
        printf("Erro: O baralho esta vazio! Nenhuma carta para usar.\n");
        return cartaVazia;
    }
    
    // 2. Isolamos o nó que está no topo antes de mexer na pilha
    NoCarta* noRemovido = pilha->topo;
    
    // Copiamos os dados do jogo para entregar de volta a quem chamou a função
    Carta cartaUsada = noRemovido->dados; 
    
    // 3. O novo topo passa a ser a carta que estava embaixo da que acabamos de tirar
    pilha->topo = noRemovido->abaixo;
    
    // 4. Liberação de Memória! (A "caixa" do nó é destruída, mas os dados já foram salvos acima)
    free(noRemovido); 
    
    printf(">> Você ativou a carta '%s'!\n", cartaUsada.nome);
    return cartaUsada;
}

/*
 * Função: exibirTopo (O Peek)
 * Objetivo: Apenas mostra ao jogador qual é a carta atual disponível para uso.
 */
void exibirTopo(PilhaCartas* pilha) {
    if (pilha->topo == NULL) {
        printf("\nO baralho esta vazio.\n");
        return;
    }
    
    printf("\n=== TOPO DO BARALHO ===\n");
    printf("Proxima carta disponivel: %s\n", pilha->topo->dados.nome);
    printf("=======================\n\n");
}