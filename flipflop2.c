#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 8

char board[BOARD_SIZE][BOARD_SIZE];

typedef struct _move
{
    int row;
    int col;
    int num_flips;
    struct _move *next;
} Move;

void init_board();
void print_board();
int is_valid_move(int row, int col, char color);
int flip_discs(int row, int col, int d_row, int d_col, char color);
void make_move(int row, int col, char color);
int count_flips(int row, int col, char color);
void get_move(int *row, int *col, char color);
char opponent_color(char color);
void game_over();
void get_ai_move(int *row, int *col, char color);
void ai_make_move(char color);
void add_move(int row, int col, int num_flips, Move **move_list);
void free_moves(Move *move_list);

char current_color = 'X';
char player_color = 'X';
char ai_color = 'O';
int save_moves = 0;
char *save_file = NULL;
int allow_undo = 0;

int main(int argc, char *argv[])
{

    // Parse command-line arguments
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
        {
            save_moves = 1;
            save_file = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
        {
            player_color = toupper(argv[i + 1][0]);
            ai_color = (player_color == 'X') ? 'O' : 'X';
            i++;
        }
        else if (strcmp(argv[i], "-u") == 0)
        {
            allow_undo = 1;
        }
    }

    init_board();

    while (1)
    {
        if (current_color == ai_color)
        {
            int row, col;
            get_ai_move(&row, &col, current_color);
            printf("AI chooses row %d, col %d.\n", row, col);
            make_move(row, col, current_color);
        }
        else
        {
            print_board();

            if (save_moves && save_file)
            {
                // TODO: Implement saving of moves to file
            }

            /*
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
            */

            int row, col;
            get_move(&row, &col, current_color);

            if (!is_valid_move(row, col, current_color))
            {
                printf("Invalid move. Try again.\n");
                continue;
            }

            make_move(row, col, current_color);
        }

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
            board[i][j] = '.';
        }
    }

    board[3][3] = 'X';
    board[4][4] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
}

void print_board()
{
    int i, j;

    printf("\n  ");
    for (j = 0; j < BOARD_SIZE; j++)
    {
        printf("%d ", j);
    }
    printf("\n");

    for (i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i);
        for (j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int is_valid_move(int row, int col, char color)
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != '.')
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

void ai_make_move(char color)
{
    int i, j, k;
    int max_flips = -1;
    int row, col;
    int **flips;

    flips = (int **)malloc(sizeof(int *) * BOARD_SIZE);
    for (i = 0; i < BOARD_SIZE; i++)
    {
        flips[i] = (int *)malloc(sizeof(int) * BOARD_SIZE);
        for (j = 0; j < BOARD_SIZE; j++)
        {
            flips[i][j] = 0;
        }
    }

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (is_valid_move(i, j, color))
            {
                for (k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if (k == 0 && l == 0)
                        {
                            continue;
                        }

                        flips[i][j] += flip_discs(i, j, k, l, opponent_color(color));
                    }
                }

                if (flips[i][j] > max_flips)
                {
                    max_flips = flips[i][j];
                    row = i;
                    col = j;
                }
            }
        }
    }

    for (i = 0; i < BOARD_SIZE; i++)
    {
        free(flips[i]);
    }
    free(flips);

    printf("AI is thinking...\n");
    printf("AI chose row %d, column %d.\n", row, col);
    make_move(row, col, color);
}

void get_move(int *row, int *col, char color)
{
    printf("Enter row and column (e.g. '3 4'):");
    scanf("%d %d", row, col);
    (*row)--;
    (*col)--;
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

    if (ai_color == 'X' && x_count > o_count)
    {
        printf("The AI player (O) has lost!\n");
    }
    else if (ai_color == 'O' && o_count > x_count)
    {
        printf("The AI player (X) has lost!\n");
    }
    else if (ai_color == 'X' && o_count > x_count)
    {
        printf("The AI player (O) has won!\n");
    }
    else if (ai_color == 'O' && x_count > o_count)
    {
        printf("The AI player (X) has won!\n");
    }
    else
    {
        printf("The game was a draw!\n");
    }
}
