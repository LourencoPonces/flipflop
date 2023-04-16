#include "flipflop.h"
#include "funcoes.c"

int main(int argc, char *argv[])
{
    char *nomeFicheiro = NULL;
    int pecaJogador = PRETO;
    int undoJogada = 0;
    FILE *fp = NULL;

    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-f") == 0 && i < argc - 1)
        {
            nomeFicheiro = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-p") == 0 && i < argc - 1)
        {
            if (strcmp(argv[i + 1], "X"))
            {
                pecaJogador = PRETO;
            }
            if (strcmp(argv[i + 1], "O"))
            {
                pecaJogador = BRANCO;
            }
            else
            {
                printf("Escolha de jogador invalida: %s\n", argv[i + 1]);
                printf("Utilizacao: ./flipflop [-f nome_ficheiro] [-p X|O] [-u]\n");
                return -1;
            }
            i += 2;
        }
        else if (strcmp(argv[i], "-u") == 0)
        {
            undoJogada = 1;
            i++;
        }
        else
        {
            printf("Argumentos invalidos: %s\n", argv[i]);
            printf("Utilizacao: ./flipflop [-f nome_ficheiro] [-p X|O] [-u]\n");
            return -1;
        }
    }

    if (nomeFicheiro != NULL)
    {
        fp = fopen(nomeFicheiro, "w");
        if (fp == NULL)
        {
            printf("Erro na abertura do ficheiro %s\n", nomeFicheiro);
            return 1;
        }
    }

    jogar(pecaJogador, undoJogada, fp);

    /*int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
   int jogadorAtual = PRETO;

   JOGADA *top = NULL;

   inicializarTabuleiro(tabuleiro);*/

    return 0;
}