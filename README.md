# FlipFlop - Reversi game in C

This project is a game developed in C language, similar to the Reversi strategy game, where the players take turns placing pieces on a board to transform the opponent's pieces.

## Game rules

The game is played on an 8x8 board, where each player has a set of pieces, with two faces, one white ('O') and one black ('X'). The players take turns placing their pieces on the board, attempting to capture their opponent's pieces by surrounding them with their own pieces. The game ends when the board is completely filled, or one player has no pieces left on the board.

## Running the game

To run the game, open a terminal and navigate to the project directory. Then, compile the code with the following command:

`gcc -Wall -Werror -pedantic -ansi \*.c -o flipflop`

To start the game, run the following command:

`./flipflop [options]`

Replace `[options]` with any of the following command-line options:

- `-f nome_ficheiro`: Saves all the game data and moves in a file.
- `-p X`: Sets the human player's piece to X or O.
- `-u`: Allows the human player to undo moves.

## Algorithm for computer player

The computer player must use an algorithm to determine the best move to make. The algorithm should:

1. Look for all possible moves that could result in capturing the opponent's pieces.
2. Determine the number of opponent's pieces that would be captured with each possible move.
3. Mark the positions on the board where a move can be made, with the 'P' symbol.
4. Choose the position that results in the most opponent's pieces captured. If multiple positions result in the same number of pieces captured, choose the one with the lowest x and y coordinates.

## Disclaimer

The project specifications may be incomplete or ambiguous, in which case the programmer may use their best judgement to fill in the gaps.

## Project statement

For more information on the project requirements, please refer to the [project statement](https://github.com/LourencoPonces/flipflop/blob/master/project_statement.pdf) provided by Instituto Superior Técnico.

## Acknowledgements

This project was developed for the course "Programação" at Instituto Superior Técnico, Lisbon, Portugal.
