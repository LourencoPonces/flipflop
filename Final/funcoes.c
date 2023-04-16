typedef struct jogada
{
    int linha;
    int coluna;
    struct jogada *jogadaAnterior;
} JOGADA;

void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
    {
        for (j = 0; j < TAMANHO_TABULEIRO; j++)
        {
            tabuleiro[i][j] = VAZIO;
        }
    }
    tabuleiro[3][3] = BRANCO;
    tabuleiro[4][4] = BRANCO;
    tabuleiro[3][4] = PRETO;
    tabuleiro[4][3] = PRETO;
}

int eJogadaValida(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int jogadorAtual, int linha, int coluna)
{
    if (linha < 0 || linha >= TAMANHO_TABULEIRO || coluna < 0 || coluna >= TAMANHO_TABULEIRO || tabuleiro[linha][coluna] != VAZIO)
    {
        return 0;
    }
    else
    {
        int linhaIncr, colunaIncr;
        int linhaAdj, colunaAdj;
        int encontrouAdvers = 0;

        for (linhaIncr = -1; linhaIncr <= 1; linhaIncr++)
        {
            for (colunaIncr = -1; colunaIncr <= 1; colunaIncr++)
            {
                if (linhaIncr != 0 || colunaIncr != 0)
                {
                    linhaAdj = linha + linhaIncr;
                    colunaAdj = coluna + colunaIncr;

                    while (linhaAdj >= 0 && linhaAdj < TAMANHO_TABULEIRO && colunaAdj >= 0 && colunaAdj < TAMANHO_TABULEIRO)
                    {
                        /*if (tabuleiro[r][c] == Empty)
                        {
                            is_valid = 0;
                        }*/
                        if (tabuleiro[linhaAdj][colunaAdj] == jogadorAtual && encontrouAdvers)
                        {
                            return 1;
                        }
                        else
                        {
                            encontrouAdvers = 1;
                            linhaAdj += linhaIncr;
                            colunaAdj += colunaIncr;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int haJogadaValida(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int jogadorAtual)
{
    int linha, coluna;
    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++)
    {
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++)
        {
            if (eJogadaValida(tabuleiro, jogadorAtual, linha, coluna))
            {
                return 1;
            }
        }
    }
    return 0;
}

void imprimirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;
    printf("  ");
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
    {
        printf("%d ", i);
        for (j = 0; j < TAMANHO_TABULEIRO; j++)
        {
            switch (tabuleiro[i][j])
            {
            case VAZIO:
                printf(". ");
                break;
            case PRETO:
                printf("X ");
                break;
            case BRANCO:
                printf("O ");
                break;
            }
        }
        printf("\n");
    }
}

int avaliarPosicao()
{
    return 1;
}

void undo()
{
}

void fazerJogada(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int jogadorAtual, int linha, int coluna, int *pontosPreto, int *pontosBranco)
{
    tabuleiro[linha][coluna] = jogadorAtual;

    int linhaIncr, colunaIncr;
    int linhaAdj, colunaAdj;
    int encontrouAdvers = 0;

    for (linhaIncr = -1; linhaIncr <= 1; linhaIncr++)
    {
        for (colunaIncr = -1; colunaIncr <= 1; colunaIncr++)
        {
            if (linhaIncr != 0 || colunaIncr != 0)
            {
                linhaAdj = linha + linhaIncr;
                colunaAdj = coluna + colunaIncr;

                encontrouAdvers = 0;
                int empty_found = 0;

                while (linhaAdj >= 0 && linhaAdj < TAMANHO_TABULEIRO && colunaAdj >= 0 && colunaAdj < TAMANHO_TABULEIRO && !empty_found)
                {
                    /*if (tabuleiro[linhaAdj][colunaAdj] == Empty)
                    {
                        empty_found = 1;
                    }*/
                    else if (tabuleiro[linhaAdj][colunaAdj] == jogadorAtual && encontrouAdvers)
                    {
                        int rr = row + linhaIncr;
                        int cc = col + colunaIncr;
                        while (rr != r || cc != c)
                        {
                            tabuleiro[rr][cc] = current_player;
                            rr += linhaIncr;
                            cc += colunaIncr;
                        }
                    }
                    else if (tabuleiro[linhaAdj][colunaAdj] != current_player)
                    {
                        encontrouAdvers = 1;
                    }
                    r += linhaIncr;
                    c += colunaIncr;
                }
            }
        }
    }
}

void jogar(int pecaJogador, int undoJogada, FILE *fp)
{

    int linha, coluna;
    int jogadorAtual = PRETO;
    JOGADA *top = NULL;
    JOGADA *novaJogada;

    int *pontosPreto = 2, *pontosBranco = 2;
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    inicializarTabuleiro(tabuleiro);

    while (haJogadaValida(tabuleiro, jogadorAtual))
    {
        imprimirTabuleiro(tabuleiro);
        /* Impressao da linha especial printf("Current player: %s\n", (current_player == Black) ? "Black" : "White");*/
        if (pecaJogador == jogadorAtual) /* Jogada do jogador */
        {
            int valid_move = 0;
            while (!valid_move)
            {
                if (scanf("%d %d", &linha, &coluna) == 2)
                {
                    fazerJogada();
                }
                else
                {
                    undo();
                }

                /*if (is_valid_move(row, col))
                {
                    valid_move = 1;
                }
                else
                {
                    printf("Jogada ilegal, porfavor tenta outra vez.\n");
                }*/
            }
        }
        else /* Jogada da IA */
        {
            int melhorValor = -1;
            int valor;
            int melhorLinha, melhorColuna;
            int i, j;
            for (i = 0; i < TAMANHO_TABULEIRO; i++)
            {
                for (j = 0; j < TAMANHO_TABULEIRO; j++)
                {
                    if (eJogadaValida(tabuleiro, jogadorAtual, i, j))
                    {
                        valor = avaliarPosicao(i, j);
                        if (valor > melhorValor)
                        {
                            melhorValor = valor;
                            melhorLinha = i;
                            melhorColuna = j;
                        }
                    }
                }
            }
            linha = melhorLinha;
            coluna = melhorColuna;
            printf("Computer played: %d %d\n", linha, coluna);
        }

        fazerJogada(tabuleiro, linha, coluna, &pontosPreto, &pontosBranco);

        if (fp != NULL)
        {
            novaJogada = (JOGADA *)calloc(1, sizeof(JOGADA));
            novaJogada->linha = linha;
            novaJogada->coluna = coluna;
            novaJogada->jogadaAnterior = top;
            top = novaJogada;
        }
    }
}