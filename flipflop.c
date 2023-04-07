#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 8

char board[BOARD_SIZE][BOARD_SIZE];

void init_board();
void print_board();
int is_valid_move(int row, int col, char color);
int flip_discs(int row, int col, int d_row, int d_col, char color);
void make_move(int row, int col, char color);
int has_valid_move(char color);
void get_move(int *row, int *col);
char opponent_color(char color);
void game_over();

void print_board()
{
    int i, j;

    printf("\n  ");
    for (j = 0; j < BOARD_SIZE; j++)
    {
        printf("%d ", j + 1);
    }
    printf("\n");

    for (i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i + 1);
        for (j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int is_valid_move(int row, int col, char color)
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != ' ')
    {
        return 0;
    }

    int d_row, d_col;
    for (d_row = -1; d_row <= 1; d_row++)
    {
        for (d_col = -1; d_col <= 1; d_col++)
        {
            if (d_row == 0 && d_col == 0)
            {
                continue;
            }

            if (flip_discs(row, col, d_row, d_col, color))
            {
                return 1;
            }
        }
    }

    return 0;
}

int flip_discs(int row, int col, int d_row, int d_col, char color)
{
    int flip_count = 0;
    char opp_color = opponent_color(color);

    int r = row + d_row;
    int c = col + d_col;

    while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == opp_color)
    {
        r += d_row;
        c += d_col;
        flip_count++;
    }

    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != color)
    {
        flip_count = 0;
    }

    if (flip_count > 0)
    {
        r = row + d_row;
        c = col + d_col;

        while (board[r][c] == opp_color)
        {
            board[r][c] = color;
            r += d_row;
            c += d_col;
        }

        board[row][col] = color;
    }

    return flip_count;
}

void make_move(int row, int col, char color)
{
    int d_row, d_col;

    for (d_row = -1; d_row <= 1; d_row++)
    {
        for (d_col = -1; d_col <= 1; d_col++)
        {
            if (d_row == 0 && d_col == 0)
            {
                continue;
            }

            flip_discs(row, col, d_row, d_col, color);
        }
    }
}

int has_valid_move(char color)
{
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (is_valid_move(i, j, color))
            {
                return 1;
            }
        }
    }

    return 0;
}

void get_move(int *row, int *col)
{
    printf("Enter row and column (e.g. '3 4'):");
    scanf("%d %d", row, col);
    (*row)--;
    (*col)--;
}

char opponent_color(char color)
{
    return (color == 'X') ? 'O' : 'X';
}

void game_over()
{
    int i, j;
    int x_count = 0, o_count = 0;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 'X')
            {
                x_count++;
            }
            else if (board[i][j] == 'O')
            {
                o_count++;
            }
        }
    }

    printf("Game over!\n");
    printf("X: %d\n", x_count);
    printf("O: %d\n", o_count);
    if (x_count > o_count)
    {
        printf("X wins!\n");
    }
    else if (o_count > x_count)
    {
        printf("O wins!\n");
    }
    else
    {
        printf("It's a tie!\n");
    }
}

int main()
{
    char current_color = 'X';

    init_board();

    while (1)
    {
        print_board();

        if (!has_valid_move(current_color))
        {
            printf("%c has no valid moves.\n", current_color);
            current_color = opponent_color(current_color);
            if (!has_valid_move(current_color))
            {
                game_over();
                return 0;
            }
        }

        int row;
        int col;
        get_move(&row, &col);

        if (!is_valid_move(row, col, current_color))
        {
            printf("Invalid move. Try again.\n");
            continue;
        }

        make_move(row, col, current_color);

        current_color = opponent_color(current_color);
    }

    return 0;
}

void init_board()
{
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = ' ';
        }
    }

    board[3][3] = 'X';
    board[4][4] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
}
