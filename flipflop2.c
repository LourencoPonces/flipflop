#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8

enum Piece
{
    Empty,
    Black,
    White
};

enum Piece board[BOARD_SIZE][BOARD_SIZE];

enum Piece current_player;

// Linked list node that represents a move on the game board
struct Move
{
    int row;
    int col;
    struct Move *prev;
};

typedef struct Move Move;

Move *head = NULL;

// Function to initialize the game board with empty spaces and initial pieces
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

// Function to print the game board to the console
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

// Function to switch the current player
void switch_player()
{
    current_player = (current_player == Black) ? White : Black;
}

// Function to check if a move is valid
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

// Function to make a move on the game board
void make_move(int row, int col)
{
    board[row][col] = current_player;
    Move *move = (Move *)malloc(sizeof(Move));
    move->row = row;
    move->col = col;
    move->prev = head;
    head = move;
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

// Function to count the number of opponent pieces that can be flipped in a certain direction
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

// Function to evaluate the value of a position on the game board
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

// Function to check if the current player has any valid moves
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

// Function to undo the last move made on the game board
void undo_move()
{
    if (head != NULL)
    {
        board[head->row][head->col] = Empty;
        Move *temp = head;
        head = head->prev;
        free(temp);
    }
}

// Function to play the game
void play_game(int save_moves, FILE *fp, char *player_choice, int undo_moves, char *file_name)
{
    int row, col;
    int valid_move;
    int black_score = 2, white_score = 2;
    initialize_board();
    if (player_choice != NULL && player_choice[0] == 'O')
    {
        current_player = White;
    }
    else
    {
        current_player = Black;
    }
    if (save_moves)
    {
        fp = fopen(file_name, "w");
    }
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
            if (save_moves && fp != NULL)
            {
                fprintf(fp, "%d %d\n", row, col);
                fflush(fp);
            }
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
            if (undo_moves)
            {
                Move *new_move = (Move *)malloc(sizeof(Move));
                new_move->row = row;
                new_move->col = col;
                new_move->prev = head;
                head = new_move;
            }
            if (save_moves && fp != NULL)
            {
                fprintf(fp, "Player %s played: %d %d\n", (current_player == Black) ? "Black" : "White", row, col);
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
        if (save_moves && fp != NULL)
        {
            fprintf(fp, "Black: %d\n", black_score);
            fprintf(fp, "White: %d\n", white_score);
        }
        switch_player();
        if (undo_moves && current_player == Black && head != NULL)
        {
            char undo_choice;
            printf("Do you want to undo the last move? (y/n): ");
            scanf(" %c", &undo_choice);
            if (undo_choice == 'y')
            {
                undo_move();
                printf("Last move undone.\n");
                if (save_moves && fp != NULL)
                {
                    fprintf(fp, "Last move undone.\n");
                }
                switch_player();
            }
        }
    }
    printf("\n");
    print_board();
    printf("\n");
    if (black_score > white_score)
    {
        printf("Black wins!\n");
        if (save_moves && fp != NULL)
        {
            fprintf(fp, "Black wins!\n");
        }
    }
    else if (white_score > black_score)
    {
        printf("White wins!\n");
        if (save_moves && fp != NULL)
        {
            fprintf(fp, "White wins!\n");
        }
    }
    else
    {
        printf("It's a tie!\n");
        if (save_moves && fp != NULL)
        {
            fprintf(fp, "It's a tie!\n");
        }
    }
    if (save_moves && fp != NULL)
    {
        fclose(fp);
    }
}

// Main function
int main(int argc, char *argv[])
{
    int save_moves = 0;
    char *file_name = NULL;
    char *player_choice = NULL;
    int undo_moves = 0;
    FILE *fp = NULL;

    // Parse command line arguments
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-f") == 0 && i < argc - 1)
        {
            save_moves = 1;
            file_name = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-p") == 0 && i < argc - 1)
        {
            player_choice = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-u") == 0)
        {
            undo_moves = 1;
            i++;
        }
        else
        {
            printf("Invalid argument: %s\n", argv[i]);
            printf("Usage: ./reversi [-f file_name] [-p X|O] [-u]\n");
            return 1;
        }
    }

    // Validate player choice argument
    if (player_choice != NULL && strcmp(player_choice, "X") != 0 && strcmp(player_choice, "O") != 0)
    {
        printf("Invalid player choice: %s\n", player_choice);
        printf("Usage: ./reversi [-f file_name] [-p X|O] [-u]\n");
        return 1;
    }

    // Open file for saving moves, if requested
    if (save_moves)
    {
        fp = fopen(file_name, "w");
        if (fp == NULL)
        {
            printf("Error opening file %s\n", file_name);
            return 1;
        }
    }

    // Start game
    play_game(save_moves, fp, player_choice, undo_moves, file_name);

    // Clean up
    if (fp != NULL)
    {
        fclose(fp);
    }
    Move *current_move = head;
    while (current_move != NULL)
    {
        Move *next_move = current_move->prev;
        free(current_move);
        current_move = next_move;
    }

    return 0;
}
