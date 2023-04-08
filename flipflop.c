#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 8

enum Piece
{
    Empty,
    Black,
    White
};

enum Piece board[BOARD_SIZE][BOARD_SIZE];

enum Piece current_player;

void initialize_board()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = Empty;
        }
    }
    board[3][3] = White;
    board[4][4] = White;
    board[3][4] = Black;
    board[4][3] = Black;
}

void print_board()
{
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            switch (board[i][j])
            {
            case Empty:
                printf(". ");
                break;
            case Black:
                printf("X ");
                break;
            case White:
                printf("O ");
                break;
            }
        }
        printf("\n");
    }
}

void switch_player()
{
    current_player = (current_player == Black) ? White : Black;
}

int is_valid_move(int row, int col)
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE ||
        board[row][col] != Empty)
    {
        return 0;
    }
    int r, c;
    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }
            r = row + dr;
            c = col + dc;
            int found_other = 0;
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE)
            {
                if (board[r][c] == Empty)
                {
                    break;
                }
                if (board[r][c] == current_player)
                {
                    if (found_other)
                    {
                        return 1;
                    }
                    break;
                }
                found_other = 1;
                r += dr;
                c += dc;
            }
        }
    }
    return 0;
}

void make_move(int row, int col)
{
    board[row][col] = current_player;
    int r, c;
    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }
            r = row + dr;
            c = col + dc;
            int found_other = 0;
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE)
            {
                if (board[r][c] == Empty)
                {
                    break;
                }
                if (board[r][c] == current_player)
                {
                    if (found_other)
                    {
                        r -= dr;
                        c -= dc;
                        while (board[r][c] != current_player)
                        {
                            board[r][c] = current_player;
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

int count_opponent_pieces(int row, int col, int dr, int dc)
{
    int r = row + dr;
    int c = col + dc;
    int count = 0;
    while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE)
    {
        if (board[r][c] == current_player)
        {
            return count;
        }
        if (board[r][c] == Empty)
        {
            return 0;
        }
        count++;
        r += dr;
        c += dc;
    }
    return 0;
}

int evaluate_position(int row, int col)
{
    int value = 0;
    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }
            value += count_opponent_pieces(row, col, dr, dc);
        }
    }
    return value;
}

int has_valid_moves()
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (is_valid_move(row, col))
            {
                return 1;
            }
        }
    }
    return 0;
}

void play_game()
{
    int row, col;
    int valid_move;
    int black_score = 2, white_score = 2;
    initialize_board();
    current_player = Black;
    while (has_valid_moves())
    {
        if (current_player == White)
        {
            int max_value = -1000;
            int best_row, best_col;
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    if (is_valid_move(i, j))
                    {
                        int value = evaluate_position(i, j);
                        if (value > max_value)
                        {
                            max_value = value;
                            best_row = i;
                            best_col = j;
                        }
                    }
                }
            }
            row = best_row;
            col = best_col;
            printf("Computer played: %d %d\n", row, col);
        }
        else
        {
            valid_move = 0;
            printf("\n");
            print_board();
            printf("\n");
            printf("Current player: %s\n", (current_player == Black) ? "Black" : "White");
            while (!valid_move)
            {
                printf("Enter row and column: ");
                scanf("%d %d", &row, &col);
                if (is_valid_move(row, col))
                {
                    valid_move = 1;
                }
                else
                {
                    printf("Invalid move, please try again.\n");
                }
            }
        }
        make_move(row, col);
        black_score = 0, white_score = 0;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == Black)
                {
                    black_score++;
                }
                else if (board[i][j] == White)
                {
                    white_score++;
                }
            }
        }
        printf("Black: %d\n", black_score);
        printf("White: %d\n", white_score);
        switch_player();
    }
    printf("\n");
    print_board();
    printf("\n");
    if (black_score > white_score)
    {
        printf("Black wins!\n");
    }
    else if (white_score > black_score)
    {
        printf("White wins!\n");
    }
    else
    {
        printf("It's a tie!\n");
    }
}

int main()
{
    play_game();
    return 0;
}
