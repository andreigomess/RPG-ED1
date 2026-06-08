#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

/*
 * Função: inicializarJogo
 * Objetivo: Executar todos os mallocs e preparar o cenário inicial.
 */

 void limparTela() {
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
            printf("\n>> Voce canaliza a magia do seu baralho...\n");
            
            // 1. Verificamos se o baralho não está vazio
            if (jogo->baralho->topo == NULL) {
                printf("[!] Seu baralho esta vazio! Derrote monstros para conseguir cartas.\n");
            } else {
                // 2. Removemos (Pop) a carta do topo da Pilha
                Carta cartaJogada = desempilharCarta(jogo->baralho);
                printf("Voce conjurou a carta: '%s'\n", cartaJogada.nome);
                
                // 3. Pedimos as coordenadas para o jogador
                int linhaEscolhida, colunaEscolhida;
                printf("Onde deseja colocar? Digite a Linha (0 a 6) e Coluna (0 a 6) separadas por espaco: ");
                scanf("%d %d", &linhaEscolhida, &colunaEscolhida);
                
                // 4. Verificamos se as coordenadas são válidas e se o terreno está vazio (0)
                if (linhaEscolhida >= 0 && linhaEscolhida < LINHAS && 
                    colunaEscolhida >= 0 && colunaEscolhida < COLUNAS &&
                    jogo->cenario->grade[linhaEscolhida][colunaEscolhida] == 0) {
                    
                    // 5. Colocamos a estrutura no mapa da matriz usando o idTerreno!
                    colocarEstrutura(jogo->cenario, linhaEscolhida, colunaEscolhida, cartaJogada.idTerreno);
                    printf(">> A estrutura '%s' emergiu magicamente no cenario!\n", cartaJogada.nome);
                    
                } else {
                    printf(">> Local invalido ou ocupado! A magia falhou e a carta virou poeira...\n");
                }
            }
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

    if (blocoAtual == jogo->mapa->sentinela) {
        printf("\n[+] VOCE VOLTOU AO ACAMPAMENTO!\n");
        printf("[+] O calor da fogueira restaura suas feridas.\n");
        
        // Cura o herói completamente
        jogo->heroi->vidaAtual = jogo->heroi->vidaMaxima; 
        
        // (Opcional) Podemos aumentar a dificuldade do jogo a cada volta aqui no futuro!
    }
    
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
                // --- INÍCIO DA TELA DE RECOMPENSAS ---
                printf("\n====================================\n");
                printf("       VITORIA! RECOMPENSAS         \n");
                printf("====================================\n");
                
                // --- 1. DROP DE ITEM ---
                int sorteioItem = rand() % 3;
                Item novoItem; 
                
                if (sorteioItem == 0) {
                    strcpy(novoItem.nome, "Espada Enferrujada");
                    novoItem.tipo = ARMA; 
                    novoItem.valorAtributo = 3;
                } else if (sorteioItem == 1) {
                    strcpy(novoItem.nome, "Escudo de Madeira");
                    novoItem.tipo = ESCUDO; 
                    novoItem.valorAtributo = 15;
                } else {
                    strcpy(novoItem.nome, "Anel da Vitalidade");
                    novoItem.tipo = ANEL; 
                    novoItem.valorAtributo = 5;
                }
                
                adicionarItem(jogo->mochila, novoItem);
                
                printf("[ITEM] %s adicionado a mochila!\n", novoItem.nome);
                if (novoItem.tipo == ARMA) {
                    jogo->heroi->poderAtaque += novoItem.valorAtributo;
                    printf("       -> Efeito: ATQ +%d\n", novoItem.valorAtributo);
                } else {
                    jogo->heroi->vidaMaxima += novoItem.valorAtributo;
                    jogo->heroi->vidaAtual += novoItem.valorAtributo; 
                    printf("       -> Efeito: HP MAX +%d\n", novoItem.valorAtributo);
                }

                printf("------------------------------------\n");

                // --- 2. DROP DE CARTA ---
                int sorteioCarta = rand() % 2; 
                Carta novaCarta;
                
                if (sorteioCarta == 0) {
                    strcpy(novaCarta.nome, "Floresta");
                    novaCarta.idTerreno = 2;
                    novaCarta.idInimigoGera = 0; 
                    novaCarta.curaVida = 0;
                } else {
                    strcpy(novaCarta.nome, "Pantano");
                    novaCarta.idTerreno = 3;
                    novaCarta.idInimigoGera = 0;
                    novaCarta.curaVida = 0;
                }
                
                // O empilharCarta já vai imprimir ">> Você ganhou a carta..."
                empilharCarta(jogo->baralho, novaCarta);
                printf("====================================\n");
                // --- FIM DA TELA DE RECOMPENSAS ---
                
            } else {
                printf(">> Sua jornada termina aqui...\n");
                jogo->jogoRodando = 0; 
                return;
            }
    }
    
    // =======================================================
    // PARTE 2: Encontros Aleatórios (Slime) + Drops!
    // =======================================================
    else if (blocoAtual->tipoTerreno == 1) { 
        
        // 20% de chance de achar um Slime
        if (rand() % 100 < 20) {
            printf("\n[!] UM SLIME SURGIU DAS SOMBRAS!\n");
            Inimigo monstroPadrao = {"Slime do Caminho", 20, 20, 4};
            
            int venceu = iniciarCombate(jogo->heroi, monstroPadrao);
            
            if (venceu == 1) {
                // --- INÍCIO DA TELA DE RECOMPENSAS ---
                printf("\n====================================\n");
                printf("       VITORIA! RECOMPENSAS         \n");
                printf("====================================\n");
                
                // --- 1. DROP DE ITEM ---
                int sorteioItem = rand() % 3;
                Item novoItem; 
                
                if (sorteioItem == 0) {
                    strcpy(novoItem.nome, "Espada Enferrujada");
                    novoItem.tipo = ARMA; 
                    novoItem.valorAtributo = 3;
                } else if (sorteioItem == 1) {
                    strcpy(novoItem.nome, "Escudo de Madeira");
                    novoItem.tipo = ESCUDO; 
                    novoItem.valorAtributo = 15;
                } else {
                    strcpy(novoItem.nome, "Anel da Vitalidade");
                    novoItem.tipo = ANEL; 
                    novoItem.valorAtributo = 5;
                }
                
                adicionarItem(jogo->mochila, novoItem);
                
                printf("[ITEM] %s adicionado a mochila!\n", novoItem.nome);
                if (novoItem.tipo == ARMA) {
                    jogo->heroi->poderAtaque += novoItem.valorAtributo;
                    printf("       -> Efeito: ATQ +%d\n", novoItem.valorAtributo);
                } else {
                    jogo->heroi->vidaMaxima += novoItem.valorAtributo;
                    jogo->heroi->vidaAtual += novoItem.valorAtributo; 
                    printf("       -> Efeito: HP MAX +%d\n", novoItem.valorAtributo);
                }

                printf("------------------------------------\n");

                // --- 2. DROP DE CARTA ---
                int sorteioCarta = rand() % 2; 
                Carta novaCarta;
                
                if (sorteioCarta == 0) {
                    strcpy(novaCarta.nome, "Floresta");
                    novaCarta.idTerreno = 2;
                    novaCarta.idInimigoGera = 0; 
                    novaCarta.curaVida = 0;
                } else {
                    strcpy(novaCarta.nome, "Pantano");
                    novaCarta.idTerreno = 3;
                    novaCarta.idInimigoGera = 0;
                    novaCarta.curaVida = 0;
                }
                
                // O empilharCarta já vai imprimir ">> Você ganhou a carta..."
                empilharCarta(jogo->baralho, novaCarta);
                printf("====================================\n");
                // --- FIM DA TELA DE RECOMPENSAS ---
                
            } else {
                printf(">> Sua jornada termina aqui...\n");
                jogo->jogoRodando = 0; 
                return;
            }
        }
    }

    // --- INÍCIO DO ESCANEAMENTO DE VIZINHOS ---
    int hLinha = blocoAtual->linhaVisual;
    int hColuna = blocoAtual->colunaVisual;
    
    // Arrays auxiliares para representar: Cima, Baixo, Esquerda, Direita na Matriz
    int deslocamentoLinha[4] = {-1, 1, 0, 0};
    int deslocamentoColuna[4] = {0, 0, -1, 1};
    
    // O herói olha para as 4 direções ao redor do bloco onde acabou de pisar
    for (int i = 0; i < 4; i++) {
        int vizinhoLinha = hLinha + deslocamentoLinha[i];
        int vizinhoColuna = hColuna + deslocamentoColuna[i];
        
        // Garante que não vamos tentar olhar para fora do mapa
        if (vizinhoLinha >= 0 && vizinhoLinha < LINHAS && 
            vizinhoColuna >= 0 && vizinhoColuna < COLUNAS) {
            
            // Pega o ID da estrutura que está nesse bloco vizinho
            int idTerrenoVizinho = jogo->cenario->grade[vizinhoLinha][vizinhoColuna];
            
            // --- EFEITOS DOS TERRENOS ---
            
            // 1. FLORESTA (ID 2)
            if (idTerrenoVizinho == 2) {
                // Sorteia uma chance de 25% de gerar um Lobo
                if (rand() % 100 < 25) {
                    printf("\n[!] Um Lobo feroz saltou da Floresta vizinha!\n");
                    
                    // Inimigo gerado on-the-fly
                    Inimigo lobo = {"Lobo da Floresta", 25, 25, 5};
                    
                    int venceu = iniciarCombate(jogo->heroi, lobo);
                    if (venceu == 0) {
                        printf(">> Voce foi devorado. Sua jornada termina aqui...\n");
                        jogo->jogoRodando = 0;
                        return; // Sai da função imediatamente
                    }
                }
            }
            
            // 2. PÂNTANO (ID 3)
            else if (idTerrenoVizinho == 3) {
                printf("\n[!] O ar pesado do Pantano vizinho sufoca voce... (-2 HP)\n");
                jogo->heroi->vidaAtual -= 2;
                
                // Verifica se o dano passivo matou o herói
                if (jogo->heroi->vidaAtual <= 0) {
                    printf(">> Voce sucumbiu aos gases toxicos. Sua jornada termina aqui...\n");
                    jogo->jogoRodando = 0;
                    return; // Sai da função
                }
            }
        }
    }
    // --- FIM DO ESCANEAMENTO DE VIZINHOS ---
}




void renderizarTela(EstadoJogo* jogo) {
    // 1. Apaga o turno passado
    limparTela();
    
    printf("==================================================\n");
    printf("                  LOOP HERO - C                   \n");
    printf("==================================================\n");
    
    // 2. Painel de Status com Barra de Vida
    printf(" [ STATUS DO AVENTUREIRO ]\n");
    printf(" Nome:   %s\n", jogo->heroi->nome);
    
    printf(" Vida:   [");
    // Calcula quantos blocos a barra deve ter (de 0 a 10)
    int blocosVida = (jogo->heroi->vidaAtual * 10) / jogo->heroi->vidaMaxima;
    for(int i = 0; i < 10; i++) {
        if(i < blocosVida) {
            printf("#"); // Parte cheia da vida
        } else {
            printf("-"); // Parte vazia da vida
        }
    }
    printf("] %d/%d\n", jogo->heroi->vidaAtual, jogo->heroi->vidaMaxima);
    
    printf(" Ataque: %d\n", jogo->heroi->poderAtaque);
    printf("==================================================\n");
    
    // 3. Painel do Local Atual
    NoMapa* local = jogo->heroi->posicaoAtual;
    printf(" [ LOCAL ATUAL ]\n");
    
    // Traduz o ID do terreno para um nome amigável
    if (local == jogo->mapa->sentinela) {
        printf(" Terreno: Acampamento (Marco Zero)\n");
    } else {
        printf(" Terreno: Caminho Padrao\n");
    }
    
    if (local->idInimigo == 0) {
        printf(" Alerta:  A area parece segura.\n");
    } else {
        printf(" Alerta:  PRESENCA INIMIGA!\n");
    }
    printf("==================================================\n");
    
    // 4. Desenha a matriz do mapa por baixo de tudo
    int hLinha = jogo->heroi->posicaoAtual->linhaVisual;
    int hColuna = jogo->heroi->posicaoAtual->colunaVisual;
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