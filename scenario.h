#ifndef SCENARIO_H
#define SCENARIO_H

// Definimos as dimensões fixas da matriz
#define LINHAS 7
#define COLUNAS 7

// Estrutura que guarda a nossa matriz 2D
typedef struct {
    int grade[LINHAS][COLUNAS];
} MatrizCenario;

// Declarações das funções
MatrizCenario* inicializarCenario();
void exibirCenario(MatrizCenario* cenario, int linhaHeroi, int colunaHeroi);

// Função para quando o jogador usar uma carta no cenário adjacente
void colocarEstrutura(MatrizCenario* cenario, int linha, int coluna, int idEstrutura);

#endif // SCENARIO_H