#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

/*
 * Função: inicializarJogo
 * Objetivo: Executar todos os mallocs e preparar o cenário inicial.
 */

 void limparTela() {
    // Se estivesse no Windows, seria system("cls");
    // Como você está no Linux/Codespaces, usamos "clear"
    system("clear");
}

EstadoJogo* inicializarJogo() {
    EstadoJogo* jogo = (EstadoJogo*) malloc(sizeof(EstadoJogo));
    
    if (jogo == NULL) {
        printf("Erro critico: Falha ao alocar o Estado do Jogo!\n");
        exit(1);
    }
    
    // Inicializamos todas as nossas estruturas de dados!
    
    jogo->mapa = inicializarMapa();
    jogo->heroi = inicializarHeroi(jogo->mapa);
    jogo->mochila = inicializarInventario();
    jogo->baralho = inicializarBaralho();
    jogo->filaInimigos = inicializarFila();
    jogo->cenario = inicializarCenario();
    
    jogo->jogoRodando = 1; // O jogo começa ativo
    
    return jogo;
}

/*
 * As 3 Fases do Game Loop (Esqueletos)
 */
void processarInput(EstadoJogo* jogo) {
    int opcao;
    
    printf("\nO que voce deseja fazer, %s?\n", jogo->heroi->nome);
    printf("1. Avancar pelo caminho (Andar)\n");
    printf("2. Abrir a Mochila (Inventario)\n");
    printf("3. Olhar a proxima carta do Baralho\n");
    printf("0. Desistir da jornada (Sair)\n");
    printf("Sua escolha: ");
    
    // Lê o número que o jogador digitou
    scanf("%d", &opcao);
    
    // Processa a escolha
    switch(opcao) {
        case 1:
            printf("\n>> Voce respira fundo e decide seguir em frente...\n");
            // O herói dá um passo pelo caminho circular!
            jogo->heroi->posicaoAtual = jogo->heroi->posicaoAtual->proximo;
            break;
            
        case 2:
            // Já temos a função pronta, é só chamar!
            exibirInventario(jogo->mochila);
            break;
            
        case 3:
            // Mostra o topo da Pilha
            exibirTopo(jogo->baralho);
            break;
            
        case 0:
            printf("\n>> Voce abandonou a jornada. Game Over.\n");
            // Isso quebra o Game Loop e encerra o jogo suavemente
            jogo->jogoRodando = 0; 
            break;
            
        default:
            printf("\n>> Opcao invalida! Os deuses não entenderam o seu pedido.\n");
            break;
    }
}

void atualizarLogica(EstadoJogo* jogo) {
    // Isolamos o bloco onde o herói está pisando agora para facilitar a leitura
    NoMapa* blocoAtual = jogo->heroi->posicaoAtual;
    
    // 1. Verificamos se há um inimigo gerado neste terreno
    // (No seu map.h, o idInimigo = 0 significa que está seguro)
    if (blocoAtual->idInimigo != 0) {
        limparTela();
        printf("\n[!] PERIGO! Um monstro espreita neste bloco!\n");
        
        // Puxamos o primeiro monstro da fila de espera!
        Inimigo monstroSorteado = desenfileirarInimigo(jogo->filaInimigos);
        
        // Iniciamos a batalha automática
        int venceu = iniciarCombate(jogo->heroi, monstroSorteado);
        
        if (venceu == 1) {
            printf(">> Voce limpou a area! O bloco agora esta seguro.\n");
            blocoAtual->idInimigo = 0; // O inimigo morre, o mapa fica limpo
        } else {
            printf(">> Sua jornada termina aqui...\n");
            jogo->jogoRodando = 0; // Herói morreu, fim de jogo
            return; // Sai da função imediatamente
        }
    }
    
    // 2. NOVO: Encontro Aleatório no caminho vazio!
    else if (blocoAtual->tipoTerreno == 1) { // 1 = Caminho padrão
        
        // Sorteia um número de 0 a 99 (Simula uma chance de 20%)
        int chance = rand() % 100; 
        
        if (chance < 20) {
            limparTela();
            printf("\n[!] UM INIMIGO SURGIU DAS SOMBRAS!\n");
            
            // Criamos o monstro padrão na hora
            Inimigo monstroPadrao = {"Slime do Caminho", 20, 20, 4};
            
            // O herói luta com ele imediatamente!
            int venceu = iniciarCombate(jogo->heroi, monstroPadrao);
            
            if (venceu == 0) {
                printf(">> Sua jornada termina aqui...\n");
                jogo->jogoRodando = 0; 
            }
        }
    }
}




void renderizarTela(EstadoJogo* jogo) {
    // 1. Apaga tudo o que aconteceu no turno passado
    limparTela();
    
    printf("==================================================\n");
    printf("                 LOOP HERO - C                    \n");
    printf("==================================================\n\n");
    
    // 2. Status do Herói
    printf("--- STATUS ---\n");
    printf("Aventureiro: %s\n", jogo->heroi->nome);
    printf("Vida: %d / %d | Ataque: %d\n", 
           jogo->heroi->vidaAtual, jogo->heroi->vidaMaxima, jogo->heroi->poderAtaque);
    printf("--------------\n\n");
    
    // 3. Informações do Local Atual
    NoMapa* local = jogo->heroi->posicaoAtual;
    printf("--- LOCAL ATUAL ---\n");
    printf("Terreno ID: %d | Estrutura ID: %d | Evento ID: %d\n", 
           local->tipoTerreno, local->idEstrutura, local->idEvento);
    
    if (local->idInimigo == 0) {
        printf("Status: A area parece segura.\n");
    } else {
        printf("Status: ALERTA! Presenca inimiga detectada!\n");
    }
    printf("-------------------\n\n");
    
    // Extrai as coordenadas do bloco atual do herói
    int hLinha = jogo->heroi->posicaoAtual->linhaVisual;
    int hColuna = jogo->heroi->posicaoAtual->colunaVisual;
    
    // Manda desenhar a matriz com o herói por cima!
    exibirCenario(jogo->cenario, hLinha, hColuna);
}

/*
 * Função: iniciarGameLoop
 * Objetivo: Manter o ciclo vivo até que 'jogoRodando' vire 0.
 */
void iniciarGameLoop(EstadoJogo* jogo) {
    // O herói entra no mapa e o jogo começa
    while (jogo->jogoRodando) {
        renderizarTela(jogo);   // 1. Limpa a tela e desenha o HUD (Interface)
        processarInput(jogo);   // 2. Pede a ação e pausa o jogo esperando você digitar
        atualizarLogica(jogo);  // 3. O herói anda e as batalhas acontecem
        
        // Pausa dramática para o jogador conseguir ler o que aconteceu
        // (Ex: ler o resultado de uma batalha antes da tela ser limpa no próximo turno)
        if (jogo->jogoRodando) {
            printf("\n[Pressione ENTER para continuar para o proximo turno...]");
            // Limpa o buffer do teclado e espera o Enter
            while(getchar() != '\n'); 
            getchar(); 
        }
    }
    
    printf("\n=== FIM DE JOGO ===\n");
}