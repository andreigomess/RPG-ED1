#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

/*
 * Função: inicializarInventario
 * Objetivo: Criar a mochila vazia para o herói[cite: 104].
 */
Inventario* inicializarInventario() {
    Inventario* inv = (Inventario*) malloc(sizeof(Inventario));
    
    if (inv == NULL) {
        printf("Erro ao alocar memoria para o inventario!\n");
        exit(1);
    }
    
    // Como a mochila está vazia, o início não aponta para nada (NULL)
    inv->inicio = NULL; 
    return inv;
}

/*
 * Função: adicionarItem
 * Objetivo: Alocar dinamicamente um novo item e inseri-lo na lista[cite: 106].
 */
void adicionarItem(Inventario* inv, Item novoItem) {
    // 1. Criamos a "caixa" (o Nó) que vai guardar o item
    NoItem* novoNo = (NoItem*) malloc(sizeof(NoItem));
    
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria para um novo item!\n");
        return;
    }
    
    // 2. Colocamos o item dentro da caixa
    novoNo->dados = novoItem;
    
    // 3. Inserção no início da lista (mais rápido e fácil)
    // O novo item aponta para quem era o primeiro da lista
    novoNo->proximo = inv->inicio;
    
    // Agora o início da lista passa a ser o nosso novo item
    inv->inicio = novoNo;
    
    printf(">> O item '%s' foi adicionado ao inventario!\n", novoItem.nome);
}

/*
 * Função: descartarItem (A MÁGICA DE LIBERAR MEMÓRIA)
 * Objetivo: Procurar um item pelo nome, remover da lista e usar o free().
 */
void descartarItem(Inventario* inv, const char* nomeDoItemParaDescartar) {
    NoItem* atual = inv->inicio;
    NoItem* anterior = NULL; // Precisamos saber quem vem antes para remendar a lista!
    
    // 1. Procuramos o item na lista
    while (atual != NULL && strcmp(atual->dados.nome, nomeDoItemParaDescartar) != 0) {
        anterior = atual;         // O anterior dá um passo
        atual = atual->proximo;   // O atual dá um passo
    }
    
    // Se o atual for NULL, chegamos ao fim da lista e não achamos o item
    if (atual == NULL) {
        printf("Item '%s' não encontrado no inventario.\n", nomeDoItemParaDescartar);
        return;
    }
    
    // 2. Achamos o item! Agora vamos removê-lo da corrente.
    
    // Caso especial: O item é o primeiro da lista?
    if (anterior == NULL) {
        inv->inicio = atual->proximo; // O início pula o item atual
    } else {
        // Caso normal: O item está no meio ou no fim
        // A pessoa de trás dá a mão para a pessoa da frente (pulando o 'atual')
        anterior->proximo = atual->proximo; 
    }
    
    // 3. Liberamos a memória de fato!
    printf(">> O item '%s' foi descartado. (Memoria liberada!)\n", atual->dados.nome);
    free(atual); 
}

/*
 * Função: exibirInventario
 * Objetivo: Listar todos os itens que o herói carrega no terminal.
 */
void exibirInventario(Inventario* inv) {
    if (inv->inicio == NULL) {
        printf("\nO inventario esta vazio.\n");
        return;
    }
    
    printf("\n=== INVENTARIO DO HEROI ===\n");
    NoItem* atual = inv->inicio;
    int contador = 1;
    
    while (atual != NULL) {
        // Imprime os dados do item
        printf("%d. [%s] | Tipo: %d | Atributo: +%d\n", 
               contador, atual->dados.nome, atual->dados.tipo, atual->dados.valorAtributo);
        
        atual = atual->proximo; // Avança para o próximo item
        contador++;
    }
    printf("===========================\n\n");
}