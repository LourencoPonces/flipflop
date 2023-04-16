#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_TABULEIRO 8
#define VAZIO 0
#define PRETO 1
#define BRANCO 2

void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
int haJogadaValida();
void jogar(int pecaJogador, int undoJogada, FILE *fp);