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
    // 1. Conta quantos itens já estão na mochila atualmente
    int contador = 0;
    NoItem* atual = inv->inicio;
    while (atual != NULL) {
        contador++;
        atual = atual->proximo;
    }

    // 2. Se já existirem 9 itens, removemos o mais antigo (o último da lista)
    if (contador >= 9) {
        if (inv->inicio != NULL) {
            // Caso especial: Só tem 1 item (não deve acontecer aqui, mas protege o código)
            if (inv->inicio->proximo == NULL) {
                free(inv->inicio);
                inv->inicio = NULL;
            } else {
                // Caso normal: Percorre até achar o penúltimo e o último nó
                NoItem* anterior = inv->inicio;
                NoItem* ultimo = inv->inicio->proximo;
                
                while (ultimo->proximo != NULL) {
                    anterior = ultimo;
                    ultimo = ultimo->proximo;
                }
                
                printf("\n[!] Mochila cheia! O item antigo '%s' virou poeira...\n", ultimo->dados.nome);
                free(ultimo);          // Libera a memória do item mais antigo
                anterior->proximo = NULL; // O penúltimo agora passa a ser o fim da linha
            }
        }
    }

    // 3. Insere o item novo normalmente no início da mochila
    NoItem* novoNo = (NoItem*) malloc(sizeof(NoItem));
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria para um novo item!\n");
        return;
    }
    novoNo->dados = novoItem;
    novoNo->proximo = inv->inicio;
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