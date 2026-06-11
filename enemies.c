#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemies.h"

/*
 * Função: inicializarFila
 * Objetivo: Criar a "sala de espera" vazia para os inimigos.
 */
FilaInimigos* inicializarFila() {
    FilaInimigos* fila = (FilaInimigos*) malloc(sizeof(FilaInimigos));
    
    if (fila == NULL) {
        printf("Erro ao alocar memoria para a fila de inimigos!\n");
        exit(1);
    }
    
    // Uma fila recém-criada está vazia, então início e fim apontam para o vazio
    fila->inicio = NULL;
    fila->fim = NULL;
    
    return fila;
}

/*
 * Função: enfileirarInimigo (O Enqueue)
 * Objetivo: Colocar um novo inimigo gerado no final da fila.
 */
void enfileirarInimigo(FilaInimigos* fila, Inimigo novoInimigo) {
    // 1. Criamos a "caixa" (Nó) para acomodar o monstro
    NoFila* novoNo = (NoFila*) malloc(sizeof(NoFila));
    
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria para o inimigo!\n");
        return;
    }
    
    // 2. Preenchemos os dados
    novoNo->dados = novoInimigo;
    novoNo->proximo = NULL; // Como ele é o último da fila, não tem ninguém atrás dele
    
    // 3. A lógica de inserção com o ponteiro 'fim'
    // Caso Especial: Se a fila estiver totalmente vazia
    if (fila->inicio == NULL) {
        fila->inicio = novoNo; // Ele é o primeiro...
        fila->fim = novoNo;    // ...e o último ao mesmo tempo!
    } else {
        // Caso Normal: Já tem monstros na fila
        fila->fim->proximo = novoNo; // O antigo último aponta para o novo monstro
        fila->fim = novoNo;          // Atualizamos a placa de 'fim' para o novo monstro
    }
    
    printf(">> O inimigo '%s' entrou no final da fila de geracao.\n", novoInimigo.nome);
}

/*
 * Função: desenfileirarInimigo (O Dequeue)
 * Objetivo: Tirar o inimigo do inicio da fila para processar e limpar a memória.
 */
Inimigo desenfileirarInimigo(FilaInimigos* fila) {
    Inimigo inimigoVazio = {"Nenhum", 0, 0, 0};
    
    // 1. Verificação de segurança
    if (fila->inicio == NULL) {
        printf("A fila esta vazia! Nenhum inimigo para gerar agora.\n");
        return inimigoVazio;
    }
    
    // 2. Isolamos o primeiro da fila
    NoFila* noRemovido = fila->inicio;
    Inimigo inimigoProcessado = noRemovido->dados;
    
    // 3. O início da fila agora passa a ser o segundo da fila
    fila->inicio = fila->inicio->proximo;
    
    // Detalhe crucial: se tiramos o ÚNICO monstro da fila, o 'fim' também deve ficar vazio
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    
    // 4. Liberação de Memória! 
    free(noRemovido);
    
    printf(">> O inimigo '%s' saiu da fila e esta pronto para o combate!\n", inimigoProcessado.nome);
    return inimigoProcessado;
}

/*
 * Função: exibirFila
 * Objetivo: Visualizar todos os inimigos que estão esperando (do primeiro ao último).
 */
void exibirFila(FilaInimigos* fila) {
    if (fila->inicio == NULL) {
        printf("\nA fila de inimigos esta vazia (Sem perigo no horizonte).\n");
        return;
    }
    
    printf("\n=== FILA DE INIMIGOS (ORDEM DE GERACAO) ===\n");
    NoFila* atual = fila->inicio;
    int posicao = 1;
    
    while (atual != NULL) {
        printf("%dº a sair: %s | HP: %d | ATQ: %d\n", 
               posicao, atual->dados.nome, atual->dados.vidaAtual, atual->dados.poderAtaque);
        atual = atual->proximo;
        posicao++;
    }
    printf("===========================================\n\n");
}