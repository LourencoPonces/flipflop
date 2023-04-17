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
                        } ISTO E NECESSARIO*/
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

void imprimirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;
    printf("   ");
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
    {
        printf(" %d ", i);
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
            case POSSIBILIDADE:
                printf("P ");
                break;
            }
        }
        printf("\n");
    }
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
                if (jogadorAtual == 1)
                {
                    return 1;
                }
                else
                {
                    tabuleiro[linha][coluna] = POSSIBILIDADE;
                }
            }
        }
    }
    return 0;
}

int count_opponent_pieces(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int jogadorAtual, int row, int col, int dr, int dc)
{
    int r = row + dr;
    int c = col + dc;
    int count = 0;
    while (r >= 0 && r < TAMANHO_TABULEIRO && c >= 0 && c < TAMANHO_TABULEIRO)
    {
        if (tabuleiro[r][c] == jogadorAtual)
        {
            return count;
        }
        if (tabuleiro[r][c] == VAZIO)
        {
            return 0;
        }
        count++;
        r += dr;
        c += dc;
    }
    return 0;
}

int avaliarPosicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int jogadorAtual, int row, int col)
{
    int value = 0;
    int dr, dc;
    for (dr = -1; dr <= 1; dr++)
    {
        for (dc = -1; dc <= 1; dc++)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }
            value += count_opponent_pieces(tabuleiro, jogadorAtual, row, col, dr, dc);
        }
    }
    return value;
}

void undo()
{
}

int mudarJogador(int jogadorAtual)
{
    if (jogadorAtual == 1)
        return 0;
    else
        return 1;
}

void fazerJogada(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int jogadorAtual, int linha, int coluna, int *pontosPreto, int *pontosBranco)
{
    /*
       int linhaAdj, colunaAdj;
       int linhaIncr, colunaIncr;
       int encontrouOutro, acabou;

       tabuleiro[linha][coluna] = jogadorAtual;

       for (linhaIncr = -1; linhaIncr <= 1; linhaIncr++)
       {
           for (colunaIncr = -1; colunaIncr <= 1; colunaIncr++)
           {
               if (linhaIncr != 0 && colunaIncr != 0)
               {
                   linhaAdj = linha + linhaIncr;
                   colunaAdj = coluna + colunaIncr;
                   encontrouOutro = 0;
                   acabou = 0;
                   while (linhaAdj >= 0 && linhaAdj < TAMANHO_TABULEIRO && colunaAdj >= 0 && colunaAdj < TAMANHO_TABULEIRO && acabou == 0)
                   {
                       if (tabuleiro[linhaAdj][colunaAdj] == VAZIO)
                       {
                           acabou = 1;
                       }
                       else if (tabuleiro[linhaAdj][colunaAdj] == jogadorAtual)
                       {
                           if (encontrouOutro)
                           {
                               linhaAdj -= linhaIncr;
                               colunaAdj -= colunaIncr;
                               printf("----\n");
                               printf("%d %d\n", linhaAdj, colunaAdj);
                               printf("----\n");
                               while (tabuleiro[linhaAdj][colunaAdj] != jogadorAtual)
                               {
                                   tabuleiro[linhaAdj][colunaAdj] = jogadorAtual;
                                   linhaAdj -= linhaIncr;
                                   colunaAdj -= colunaIncr;
                               }
                           }
                           acabou = 1;
                       }
                       encontrouOutro = 1;
                       linhaAdj += linhaIncr;
                       colunaAdj += colunaIncr;
                   }
               }
           }
       }
       */
    int r, c;
    int dr, dc;
    int found_other;

    tabuleiro[linha][coluna] = jogadorAtual;

    for (dr = -1; dr <= 1; dr++)
    {
        for (dc = -1; dc <= 1; dc++)
        {
            if (dr != 0 || dc != 0)
            {
                r = linha + dr;
                c = coluna + dc;
                found_other = 0;
                while (r >= 0 && r < TAMANHO_TABULEIRO && c >= 0 && c < TAMANHO_TABULEIRO)
                {
                    if (tabuleiro[r][c] == VAZIO)
                    {
                        break;
                    }
                    if (tabuleiro[r][c] == jogadorAtual)
                    {
                        if (found_other)
                        {
                            r -= dr;
                            c -= dc;
                            while (tabuleiro[r][c] != jogadorAtual)
                            {
                                tabuleiro[r][c] = jogadorAtual;
                                r -= dr;
                                c -= dc;
                            }
                        }
                        break;
                    }
                    found_other = 1;
                    r += dr;
                    c += dc;
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

    int pontosPreto = 2, pontosBranco = 2;

    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    inicializarTabuleiro(tabuleiro);
    imprimirTabuleiro(tabuleiro);

    while (haJogadaValida(tabuleiro, jogadorAtual))
    {
        if (jogadorAtual)
        {
            printf("Jogador 1 (X) (linha e coluna) = ");
        }
        else
        {
            printf("Jogador 2 (O) (linha e coluna) = ");
        }

        if (pecaJogador == jogadorAtual) /* Jogada do jogador */
        {
            int valid_move = 0;
            while (!valid_move)
            {
                if (scanf("%d %d", &linha, &coluna) == 2)
                {
                    /*fazerJogada();*/
                    if (eJogadaValida(tabuleiro, jogadorAtual, linha, coluna))
                    {
                        fazerJogada(tabuleiro, jogadorAtual, linha, coluna, &pontosPreto, &pontosBranco);
                        valid_move = 1;
                    }
                    else
                    {
                        printf("Jogada ilegal, porfavor tenta outra vez.\n");
                    }
                }
                else
                {
                    undo();
                }
            }
        }
        else /* Jogada da IA */
        {
            /*Pint melhorValor = -1;
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
            printf("%d %d\n", linha, coluna);
            */
            int max_value = -1000;
            int best_row, best_col;
            int i, j;
            for (i = 0; i < TAMANHO_TABULEIRO; i++)
            {
                for (j = 0; j < TAMANHO_TABULEIRO; j++)
                {
                    if (eJogadaValida(tabuleiro, jogadorAtual, i, j))
                    {
                        int value = avaliarPosicao(tabuleiro, jogadorAtual, i, j);
                        if (value > max_value)
                        {
                            max_value = value;
                            best_row = i;
                            best_col = j;
                        }
                    }
                }
            }
            linha = best_row;
            coluna = best_col;
            printf("Computer played: %d %d\n", linha, coluna);
        }
        jogadorAtual = mudarJogador(jogadorAtual);

        printf("\n");
        imprimirTabuleiro(tabuleiro);

        printf("-------------------------------------\n\n");

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
