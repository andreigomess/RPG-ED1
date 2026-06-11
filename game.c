#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "game.h"

/*
 * Função: inicializarJogo
 * Objetivo: Executar todos os mallocs e preparar o cenário inicial.
 */

 void limparTela() {
    system("clear");
}

int lerTecla() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Salva as configurações normais do terminal
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    // Desliga o modo canônico (exigir ENTER) e o echo (imprimir a letra)
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Liga o modo "não-bloqueante"
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Devolve o terminal ao normal para que os menus funcionem depois
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
}

// Lê uma única tecla instantaneamente e congela o jogo até você apertar
int lerTeclaMenu() {
    struct termios oldt, newt;
    int ch;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // Desliga o modo de exigir ENTER, mas MANTÉM a função bloqueante
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    ch = getchar(); // Fica travado aqui até alguma tecla ser pressionada
    
    // Devolve o terminal ao normal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

EstadoJogo* inicializarJogo() {
    
    EstadoJogo* jogo = (EstadoJogo*) malloc(sizeof(EstadoJogo));
    
    if (jogo == NULL) {
        printf("Erro critico: Falha ao alocar o Estado do Jogo!\n");
        exit(1);
    }

    // Inicializamos todas as nossas estruturas de dados!
    jogo->voltasCompletas = 0;
    jogo->mapa = inicializarMapa();
    jogo->heroi = inicializarHeroi(jogo->mapa);
    jogo->mochila = inicializarInventario();
    jogo->baralho = inicializarBaralho();
    jogo->filaInimigos = inicializarFila();
    jogo->cenario = inicializarCenario();
    
    jogo->jogoRodando = 1; // O jogo começa ativo
    
    return jogo;
}

// Função exclusiva para desenhar o mapa durante a colocação de cartas
void renderizarModoColocacao(MatrizCenario* cenario, int cursorL, int cursorC, Carta carta) {
    system("clear || cls");
    printf("==================================================\n");
    printf(" [ " ANSI_AMARELO "MODO DE CONSTRUCAO" ANSI_RESET " ]\n");
    printf(" Posicionando: '" ANSI_BRANCO_BRILHANTE "%s" ANSI_RESET "'\n", carta.nome);
    printf(" Controles: SETAS para mover | ESPACO para confirmar | 'Q' para cancelar\n");
    printf("==================================================\n\n");

    for (int i = 0; i < LINHAS; i++) {
        printf("    "); 
        for (int j = 0; j < COLUNAS; j++) {
            
            // É a posição do cursor?
            if (i == cursorL && j == cursorC) {
                if (cenario->grade[i][j] == 0) {
                    // Verde brilhante se puder colocar
                    printf(ANSI_VERDE "[*] " ANSI_RESET); 
                } else {
                    // Vermelho se estiver ocupado
                    printf(ANSI_VERMELHO "[X] " ANSI_RESET); 
                }
            } 
            // Não é o cursor, desenha as estruturas coloridas
            else {
                if (cenario->grade[i][j] == 0) {
                    printf(ANSI_CINZA " .  " ANSI_RESET); // Espaço vazio discreto
                } else if (cenario->grade[i][j] == 1) {
                    printf(ANSI_CINZA " #  " ANSI_RESET); // O Caminho (Cinza)
                } else if (cenario->grade[i][j] == 2) {
                    printf(ANSI_VERDE " F  " ANSI_RESET); // Floresta (Verde)
                } else if (cenario->grade[i][j] == 3) {
                    printf(ANSI_MAGENTA " P  " ANSI_RESET); // Pântano (Roxo)
                } else {
                    printf(" %d  ", cenario->grade[i][j]);
                }
            }
        }
        printf("\n\n");
    }
}

/*
 * As 3 Fases do Game Loop (Esqueletos)
 */
void processarInput(EstadoJogo* jogo) {
    int noMenu = 1;
    
    // Limpa qualquer sujeira do teclado antes de abrir o menu
    tcflush(STDIN_FILENO, TCIFLUSH); 

    while (noMenu && jogo->jogoRodando) {
        printf("\n==================================================\n");
        printf("             [ O TEMPO ESTA CONGELADO ]           \n");
        printf("==================================================\n");
        printf("O que voce deseja fazer, %s?\n", jogo->heroi->nome);
        printf(" [ESPACO] Retornar a Expedicao (Despausar)\n");
        printf(" [2] Abrir a Mochila (Inventario)\n");
        printf(" [3] Jogar uma Carta do Baralho\n");
        printf(" [4] Salvar Progresso (Gravar HD)\n"); 
        printf(" [5] Carregar Progresso (Ler HD)\n");
        printf(" [0] Desistir da jornada (Sair)\n");
        printf("==================================================\n");
        printf("Aperte a tecla desejada: ");
        
        // Lê a tecla na hora, sem precisar de ENTER!
        int opcao = lerTeclaMenu(); 
        
        switch(opcao) {
            case ' ': // Se apertou a Barra de Espaço
                printf("\n>> Voce descongela o tempo e a caminhada continua...\n");
                noMenu = 0; // Quebra o laço do menu e volta pro jogo
                break;
                
            case '2':
                // 1. Mostra a mochila inteira (numerada)
                exibirInventario(jogo->mochila);
                
                // 2. Mostra o que o herói está vestindo no momento
                printf("[ SEUS EQUIPAMENTOS ATUAIS ]\n");
                printf(" Arma:     %s (+%d)\n", jogo->heroi->armaEquipada.tipo ? jogo->heroi->armaEquipada.nome : "Vazio", jogo->heroi->armaEquipada.tipo ? jogo->heroi->armaEquipada.valorAtributo : 0);
                printf(" Armadura: %s (+%d)\n", jogo->heroi->armaduraEquipada.tipo ? jogo->heroi->armaduraEquipada.nome : "Vazio", jogo->heroi->armaduraEquipada.tipo ? jogo->heroi->armaduraEquipada.valorAtributo : 0);
                printf(" Escudo:   %s (+%d)\n", jogo->heroi->escudoEquipado.tipo ? jogo->heroi->escudoEquipado.nome : "Vazio", jogo->heroi->escudoEquipado.tipo ? jogo->heroi->escudoEquipado.valorAtributo : 0);
                printf(" Anel:     %s (+%d)\n", jogo->heroi->anelEquipado.tipo ? jogo->heroi->anelEquipado.nome : "Vazio", jogo->heroi->anelEquipado.tipo ? jogo->heroi->anelEquipado.valorAtributo : 0);
                printf("--------------------------------------------------\n");
                
                // 3. Lê o comando do jogador
                printf("Digite o numero do item para equipar (ou 0 para voltar): ");
                int escolha;
                scanf("%d", &escolha);
                while(getchar() != '\n'); // Limpa o "ENTER" para não dar conflito com o menu de pausa
                
                if (escolha > 0) {
                    // Percorre a mochila até encontrar o item pelo número digitado
                    NoItem* atual = jogo->mochila->inicio;
                    int i = 1;
                    while (atual != NULL && i < escolha) {
                        atual = atual->proximo;
                        i++;
                    }
                    
                    if (atual != NULL) {
                        Item itemNovo = atual->dados;
                        
                        // 4. A Lógica da Troca (Swap)
                        if (itemNovo.tipo == ARMA) {
                            if (jogo->heroi->armaEquipada.tipo != 0) {
                                // Tira o bônus da arma velha!
                                jogo->heroi->poderAtaque -= jogo->heroi->armaEquipada.valorAtributo; 
                            }
                            jogo->heroi->poderAtaque += itemNovo.valorAtributo; // Põe o bônus da nova
                            jogo->heroi->armaEquipada = itemNovo; // Veste o item fisicamente
                            
                        } else if (itemNovo.tipo == ARMADURA) {
                            if (jogo->heroi->armaduraEquipada.tipo != 0) {
                                jogo->heroi->vidaMaxima -= jogo->heroi->armaduraEquipada.valorAtributo;
                                jogo->heroi->vidaAtual -= jogo->heroi->armaduraEquipada.valorAtributo;
                            }
                            jogo->heroi->vidaMaxima += itemNovo.valorAtributo;
                            jogo->heroi->vidaAtual += itemNovo.valorAtributo;
                            jogo->heroi->armaduraEquipada = itemNovo;
                            
                        } else if (itemNovo.tipo == ESCUDO) {
                            if (jogo->heroi->escudoEquipado.tipo != 0) {
                                jogo->heroi->vidaMaxima -= jogo->heroi->escudoEquipado.valorAtributo;
                                jogo->heroi->vidaAtual -= jogo->heroi->escudoEquipado.valorAtributo;
                            }
                            jogo->heroi->vidaMaxima += itemNovo.valorAtributo;
                            jogo->heroi->vidaAtual += itemNovo.valorAtributo;
                            jogo->heroi->escudoEquipado = itemNovo;
                            
                        } else if (itemNovo.tipo == ANEL) {
                            if (jogo->heroi->anelEquipado.tipo != 0) {
                                jogo->heroi->vidaMaxima -= jogo->heroi->anelEquipado.valorAtributo;
                                jogo->heroi->vidaAtual -= jogo->heroi->anelEquipado.valorAtributo;
                            }
                            jogo->heroi->vidaMaxima += itemNovo.valorAtributo;
                            jogo->heroi->vidaAtual += itemNovo.valorAtributo;
                            jogo->heroi->anelEquipado = itemNovo;
                        }
                        
                        // 5. Destrói o item novo da mochila (pois foi para o corpo)
                        // Como nós não salvamos o item velho na mochila em nenhum lugar, ele é apagado da existência (exatamente como você pediu!)
                        descartarItem(jogo->mochila, itemNovo.nome);
                        printf("\n>> Sucesso! Voce equipou: '%s'\n", itemNovo.nome);
                    } else {
                        printf("\n>> Item não encontrado.\n");
                    }
                }
                
                printf("\n[Pressione ENTER para voltar ao menu de pausa...]");
                getchar();
                break;
                
            case '3':
                if (jogo->baralho->topo == NULL) {
                    printf("\n[!] Seu baralho esta vazio!\n");
                    printf("\n[Pressione ENTER para voltar...]");
                    getchar();
                } else {
                    Carta cartaJogada = desempilharCarta(jogo->baralho);
                    
                    int cursorL = 0; // O cursor começa na linha 0
                    int cursorC = 0; // O cursor começa na coluna 0
                    int colocando = 1;
                    
                    while (colocando) {
                        // Desenha a tela com o alvo
                        renderizarModoColocacao(jogo->cenario, cursorL, cursorC, cartaJogada);
                        
                        // Lê a tecla pressionada
                        int tecla = lerTeclaMenu();
                        
                        // VERIFICA AS SETAS DO TECLADO
                        if (tecla == 27) { // 27 é o código do ESC (Início da seta)
                            int seq1 = lerTeclaMenu();
                            int seq2 = lerTeclaMenu();
                            
                            if (seq1 == '[') {
                                // Movimenta garantindo que não saia dos limites (0 a LINHAS/COLUNAS)
                                if (seq2 == 'A' && cursorL > 0) cursorL--;             // CIMA
                                else if (seq2 == 'B' && cursorL < LINHAS - 1) cursorL++; // BAIXO
                                else if (seq2 == 'C' && cursorC < COLUNAS - 1) cursorC++; // DIREITA
                                else if (seq2 == 'D' && cursorC > 0) cursorC--;           // ESQUERDA
                            }
                        }
                        // Suporte alternativo para W, A, S, D
                        else if (tecla == 'w' || tecla == 'W') { if (cursorL > 0) cursorL--; }
                        else if (tecla == 's' || tecla == 'S') { if (cursorL < LINHAS - 1) cursorL++; }
                        else if (tecla == 'd' || tecla == 'D') { if (cursorC < COLUNAS - 1) cursorC++; }
                        else if (tecla == 'a' || tecla == 'A') { if (cursorC > 0) cursorC--; }
                        
                        // CONFIRMAR (ESPAÇO)
                        else if (tecla == ' ') {
                            if (jogo->cenario->grade[cursorL][cursorC] == 0) {
                                colocarEstrutura(jogo->cenario, cursorL, cursorC, cartaJogada.idTerreno);
                                printf("\n>> A estrutura '%s' emergiu magicamente no cenario!\n", cartaJogada.nome);
                                colocando = 0; // Sai do modo de construção
                                
                                printf("\n[Pressione ENTER para voltar ao menu de pausa...]");
                                // Usa um getchar limpo pq não usamos scanf aqui
                                char limpar = getchar(); 
                                while(limpar != '\n' && limpar != EOF) limpar = getchar();
                            }
                        }
                        
                        // CANCELAR (Q)
                        else if (tecla == 'q' || tecla == 'Q') {
                            printf("\n>> Voce guardou a carta de volta no baralho.\n");
                            // Devolve a carta para o topo, já que ele desistiu de usar
                            empilharCarta(jogo->baralho, cartaJogada); 
                            colocando = 0; // Sai do modo de construção
                            
                            printf("\n[Pressione ENTER para voltar ao menu de pausa...]");
                            char limpar = getchar();
                            while(limpar != '\n' && limpar != EOF) limpar = getchar();
                        }
                    }
                }
                break;
                
                printf("\n[Pressione ENTER para voltar ao menu de pausa...]");
                getchar();
                break;

            case '4':
                salvarJogo(jogo);
                printf("\n[Pressione ENTER para voltar ao menu de pausa...]");
                getchar();
                break;
                
            case '5':
                carregarJogo(jogo);
                printf("\n[Pressione ENTER para voltar ao menu de pausa...]");
                getchar();
                break;
                
            case '0':
                printf("\n>> Voce abandonou a jornada. Game Over.\n");
                jogo->jogoRodando = 0; 
                noMenu = 0;
                break;
                
            default:
                // Se apertar qualquer outra tecla inútil, o menu só recarrega
                break;
        }
    }
}

void atualizarLogica(EstadoJogo* jogo) {
    // Isolamos o bloco onde o herói está pisando agora para facilitar a leitura
    NoMapa* blocoAtual = jogo->heroi->posicaoAtual;

    // =======================================================
    // PARTE 0: O Acampamento (Marco Zero) + BOSS FIGHT!
    // =======================================================
    if (blocoAtual == jogo->mapa->sentinela) {
        jogo->voltasCompletas++; // Conta a nova volta!
        
        printf("\n" ANSI_AMARELO "==================================================" ANSI_RESET "\n");
        printf(" [+] VOCE COMPLETOU A VOLTA %d!\n", jogo->voltasCompletas);
        printf(" [+] O calor da fogueira restaura suas feridas.\n");
        printf(ANSI_AMARELO "==================================================" ANSI_RESET "\n");
        
        // Cura o herói completamente
        jogo->heroi->vidaAtual = jogo->heroi->vidaMaxima; 
        
        // VERIFICAÇÃO DO CHEFE (A cada 5 voltas)
        if (jogo->voltasCompletas % 5 == 0) {
            printf("\n" ANSI_VERMELHO "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ANSI_RESET "\n");
            printf(ANSI_VERMELHO "          ALERTA: O BOSS DO LOOP DESPERTOU!       " ANSI_RESET "\n");
            printf(ANSI_VERMELHO "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ANSI_RESET "\n");
            printf(" O ceu fica roxo e o solo treme... O LICH bloqueia seu caminho!\n\n");
            
            // Atributos base do Boss altos, que também escalam se você chegar na volta 10, 15...
            int hpBoss = 150 + (jogo->voltasCompletas * 15);
            int atqBoss = 16 + (jogo->voltasCompletas * 3);
            Inimigo boss = {"O Lich Supremo", hpBoss, hpBoss, atqBoss};
            
            // Força o combate imediatamente na fogueira
            int venceu = iniciarCombate(jogo->heroi, boss);
            
            if (venceu == 0) {
                printf(ANSI_VERMELHO ">> Voce foi pulverizado pelo %s. Fim do jogo.\n" ANSI_RESET, boss.nome);
                jogo->jogoRodando = 0;
                return;
            } else {
                printf("\n" ANSI_VERDE "==================================================" ANSI_RESET "\n");
                printf("  INCRIVEL! VOCE DERROTOU O LICH SUPREMO!  \n");
                printf("  A jornada continua para loops ainda mais perigosos...\n");
                printf(ANSI_VERDE "==================================================" ANSI_RESET "\n");
                
                // Recompensa lendária por matar o Boss (Coloca um item fortíssimo direto na mochila)
                Item itemLendario = {"Espada de Luz do Lich", ARMA, 12};
                adicionarItem(jogo->mochila, itemLendario);
                
                printf("\n[Pressione ENTER para continuar a expedição...]");
                char limpar = getchar();
                while(limpar != '\n' && limpar != EOF) limpar = getchar();
            }
        }
    }
    
    // 1. Verificamos se há um inimigo gerado neste terreno
    // (No seu map.h, o idInimigo = 0 significa que está seguro)
    if (blocoAtual->idInimigo != 0) {
        limparTela();
        printf("\n[!] PERIGO! Um monstro espreita neste bloco!\n");

        // O Lobo ganha +5 de HP e +2 de ATQ por volta
                    int hpLobo = 25 + (jogo->voltasCompletas * 5);
                    int atqLobo = 5 + (jogo->voltasCompletas * 2);
                    
                    Inimigo lobo = {"Lobo da Floresta", hpLobo, hpLobo, atqLobo};
                    enfileirarInimigo(jogo->filaInimigos, lobo);
        
        // Iniciamos a batalha automática
        int venceu = iniciarCombate(jogo->heroi, lobo);
        
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
            // Em vez de HP 20 fixo, o Slime ganha +4 de HP e +1 de ATQ por volta
            int hpSlime = 20 + (jogo->voltasCompletas * 4);
            int atqSlime = 4 + (jogo->voltasCompletas * 1);
            
            Inimigo monstroPadrao = {"Slime do Caminho", hpSlime, hpSlime, atqSlime};
            
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
    int modoPlanejamento = 0; // 0 = Expedição rodando, 1 = Pausado no acampamento

    while (jogo->jogoRodando) {
        // 1. O jogo "ouve" rapidamente se você bateu na barra de espaço
        int tecla = lerTecla();
        if (tecla == ' ') {
            modoPlanejamento = 1;
        }

        // 2. Decide o que fazer com base no estado do tempo
        if (modoPlanejamento) {
            
            // Limpa qualquer lixo do teclado antes de abrir o menu
            tcflush(STDIN_FILENO, TCIFLUSH); 
            
            // Congela o tempo e abre o menu de planejamento
            processarInput(jogo);
            
            // Assim que o jogador sai do menu, despausa o jogo
            modoPlanejamento = 0; 
            
        } else {
            // === MODO EXPEDIÇÃO (A MÁGICA AUTOMÁTICA) ===
            
            // 1. Desenha o mapa e o status na tela
            renderizarTela(jogo);
            
            // 2. O herói avança sozinho
            jogo->heroi->posicaoAtual = jogo->heroi->posicaoAtual->proximo;
            
            // 3. Atualiza o mundo (se houver batalhas, o texto aparecerá abaixo do mapa)
            atualizarLogica(jogo);
            
            // 4. O jogo dorme por 1 segundo, permitindo que você leia o que aconteceu 
            // e dando o ritmo do Loop Hero
            sleep(1); 
        }
    }
}

void salvarJogo(EstadoJogo* jogo) {
    FILE* arquivo = fopen("save.dat", "wb"); // wb = Write Binary (Escrita Binária)
    if (arquivo == NULL) {
        printf("\n[!] Erro ao criar o arquivo de save!\n");
        return;
    }

    DadosSave dados;
    
    // 1. Copia os atributos e itens equipados do herói
    strcpy(dados.nome, jogo->heroi->nome);
    dados.vidaMaxima = jogo->heroi->vidaMaxima;
    dados.vidaAtual = jogo->heroi->vidaAtual;
    dados.poderAtaque = jogo->heroi->poderAtaque;
    dados.armaEquipada = jogo->heroi->armaEquipada;
    dados.armaduraEquipada = jogo->heroi->armaduraEquipada;
    dados.escudoEquipado = jogo->heroi->escudoEquipado;
    dados.anelEquipado = jogo->heroi->anelEquipado;

    // 2. Copia o progresso do jogo
    dados.voltasCompletas = jogo->voltasCompletas;

    // 3. Compacta a lista encadeada da mochila para dentro do array estático
    dados.qtdItensMochila = 0;
    NoItem* atual = jogo->mochila->inicio;
    while (atual != NULL && dados.qtdItensMochila < 9) {
        dados.itensMochila[dados.qtdItensMochila] = atual->dados;
        dados.qtdItensMochila++;
        atual = atual->proximo;
    }

    // 4. Grava tudo no disco rígido com um único comando atômico
    fwrite(&dados, sizeof(DadosSave), 1, arquivo);
    fclose(arquivo);

    printf("\n" ANSI_VERDE "[+] PROGRESSO SALVO COM SUCESSO EM 'save.dat'!" ANSI_RESET "\n");
}

void carregarJogo(EstadoJogo* jogo) {
    FILE* arquivo = fopen("save.dat", "rb"); // rb = Read Binary (Leitura Binária)
    if (arquivo == NULL) {
        printf("\n" ANSI_VERMELHO "[!] Nenhum arquivo de save encontrado!" ANSI_RESET "\n");
        return;
    }

    DadosSave dados;
    fread(&dados, sizeof(DadosSave), 1, arquivo);
    fclose(arquivo);

    // 1. Restaura o herói
    strcpy(jogo->heroi->nome, dados.nome);
    jogo->heroi->vidaMaxima = dados.vidaMaxima;
    jogo->heroi->vidaAtual = dados.vidaAtual;
    jogo->heroi->poderAtaque = dados.poderAtaque;
    jogo->heroi->armaEquipada = dados.armaEquipada;
    jogo->heroi->armaduraEquipada = dados.armaduraEquipada;
    jogo->heroi->escudoEquipado = dados.escudoEquipado;
    jogo->heroi->anelEquipado = dados.anelEquipado;

    // 2. Restaura o jogo
    jogo->voltasCompletas = dados.voltasCompletas;

    // 3. Limpa a mochila atual da memória RAM para evitar vazamentos (Memory Leak)
    while (jogo->mochila->inicio != NULL) {
        NoItem* temp = jogo->mochila->inicio;
        jogo->mochila->inicio = jogo->mochila->inicio->proximo;
        free(temp);
    }

    // 4. Reconstrói a mochila inserindo os itens de trás para frente para manter a ordem exata
    for (int i = dados.qtdItensMochila - 1; i >= 0; i--) {
        adicionarItem(jogo->mochila, dados.itensMochila[i]);
    }

    // Regra de Ouro: Reposiciona o herói com segurança logo após a fogueira
    jogo->heroi->posicaoAtual = jogo->mapa->sentinela->proximo;

    printf("\n" ANSI_VERDE "[+] JOGO CARREGADO COM SUCESSO! EXPEDICAO RECONSTRUIDA." ANSI_RESET "\n");
}

void destruirJogo(EstadoJogo* jogo) {
    printf("\n>> Limpando a memoria do sistema...\n");

    // 1. Limpa o Herói
    if (jogo->heroi != NULL) {
        free(jogo->heroi);
    }

    // 2. Limpa a Mochila
    if (jogo->mochila != NULL) {
        while (jogo->mochila->inicio != NULL) {
            NoItem* temp = jogo->mochila->inicio;
            jogo->mochila->inicio = jogo->mochila->inicio->proximo;
            free(temp);
        }
        free(jogo->mochila);
    }

    // 3. Limpa o Baralho
    if (jogo->baralho != NULL) {
        while (jogo->baralho->topo != NULL) {
            NoCarta* temp = jogo->baralho->topo;
            jogo->baralho->topo = jogo->baralho->topo->abaixo;
            free(temp);
        }
        free(jogo->baralho);
    }

    // 4. Limpa a Fila de Inimigos (PERFEITO, conforme o seu enemies.h)
    if (jogo->filaInimigos != NULL) {
        while (jogo->filaInimigos->inicio != NULL) {
            NoFila* temp = jogo->filaInimigos->inicio;
            jogo->filaInimigos->inicio = jogo->filaInimigos->inicio->proximo;
            free(temp);
        }
        free(jogo->filaInimigos);
    }

    // 5. Limpa o Mapa Circular (CORRIGIDO: Percorrendo os nós primeiro)
    if (jogo->mapa != NULL) {
        if (jogo->mapa->sentinela != NULL) {
            NoMapa* atual = jogo->mapa->sentinela->proximo;
            
            // Dá a volta no mapa apagando todos os blocos do caminho
            while (atual != jogo->mapa->sentinela) {
                NoMapa* temp = atual;
                atual = atual->proximo;
                free(temp);
            }
            // Por fim, apaga a própria sentinela
            free(jogo->mapa->sentinela);
        }
        free(jogo->mapa); // Agora sim podemos apagar o gerenciador
    }

    // 6. Limpa o Cenário
    if (jogo->cenario != NULL) {
        free(jogo->cenario);
    }

    // 7. Por fim, limpa a estrutura principal
    free(jogo);
    printf(">> Memoria liberada com sucesso. Adeus!\n");
}