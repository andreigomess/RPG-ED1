#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"

int main() {
    printf("Iniciando o sistema de inventario...\n\n");

    // 1. Criamos a mochila (Lista inicia apontando para NULL)
    Inventario* minhaMochila = inicializarInventario();

    // 2. Criamos alguns itens para o teste
    // Usamos o formato: {"Nome", Tipo, Valor do Atributo}
    Item espada = {"Espada de Ferro", ARMA, 15};
    Item armadura = {"Cota de Malha", ARMADURA, 25};
    Item pocao = {"Pocao de Vida", CONSUMIVEL, 50};

    // 3. Adicionamos os itens na mochila
    // Como a inserção é no início, o último a entrar será o primeiro da lista
    adicionarItem(minhaMochila, espada);
    adicionarItem(minhaMochila, armadura);
    adicionarItem(minhaMochila, pocao);

    // 4. Exibimos a mochila cheia
    exibirInventario(minhaMochila);

    // 5. Simulamos o uso/descarte de um item
    printf("O heroi decidiu beber a pocao e descartar o frasco...\n");
    
    // Aqui a mágica do 'free()' acontece!
    descartarItem(minhaMochila, "Pocao de Vida");

    // 6. Exibimos a mochila atualizada para confirmar a remoção
    exibirInventario(minhaMochila);

    // Tentamos descartar um item que não existe para testar a segurança
    descartarItem(minhaMochila, "Escudo de Madeira");

    printf("\nFim do teste de inventario.\n");
    return 0;
}