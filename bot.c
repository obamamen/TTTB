#include <stdio.h>
#include <limits.h>

#define draw -50000

typedef enum {
    p1 = 1,
    p2 = -1,
    empty = 0
} player;

typedef struct {
    player board[9];
    player playerTurn;
} game;

void initialize(game* game,  const player p) {
    for (int i = 0; i < 9; i++) {
            game->board[i] = empty;
    }
    game->playerTurn = p;
}

char playerToChar(const player p) {
    switch (p) {
        case p1:
            return 'X';
        case p2:
            return 'O';
        default:
            return ' ';
    }
}

void print(const game* game) {
    for (int i = 0; i < 9; i++) {
        if ((i % 3 == 0) && (i != 0)) {
            printf("\n");
            printf("------------\n");
        }
        printf("%c | ", playerToChar(game->board[i]));
    }
    printf("\n");
}

player checkWin(const game* game) {
    const int winConditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        if (game->board[winConditions[i][0]] == p1 &&
            game->board[winConditions[i][1]] == p1 &&
            game->board[winConditions[i][2]] == p1) {
                return p1;
            }
        if (game->board[winConditions[i][0]] == p2 &&
            game->board[winConditions[i][1]] == p2 &&
            game->board[winConditions[i][2]] == p2) {
                return p2;
            }
    }
    for (int i = 0; i < 9; i++) {
        if (game->board[i] == empty) {
            return empty;
        }
    }
    return empty;
}



void makeMove(game* game, const int pos) {
    if (game->board[pos] != empty) {
        printf("Illegal move\n");
        return;
    }
    game->board[pos] = game->playerTurn;
    if (game->playerTurn == p1) {
        game->playerTurn = p2;
        return;
    }
    if (game->playerTurn == p2) {
        game->playerTurn = p1;
    }
}

void undoMove(game* game, const int pos) {
    game->board[pos] = empty;
    if (game->playerTurn == p1) {
        game->playerTurn = p2;
        return;
    }
    if (game->playerTurn == p2) {
        game->playerTurn = p1;
        return;
    }
}

int getTurn() {
    printf("make move: ");
    int move;
    scanf("%d", &move);
    printf("\n");
    return move-1;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int miniMax(game* game, const int depth, const int isMax) {
    const player result = checkWin(game);

    if (result == p1) {
        return 10;
    }
    if (result == p2) {
        return -10;
    }

    if (isMax == 1) {
        int best = INT_MIN;
        for (int i = 0; i < 9; i++) {
            if (game->board[i] == empty) {
                game->board[i] = p1;
                int value = miniMax(game, depth + 1, 0);
                game->board[i] = empty;
                best = max(best, value);
            }
        }
        if (best == INT_MIN) {
            return empty;
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 9; i++) {
            if (game->board[i] == empty) {
                game->board[i] = p2;
                int value = miniMax(game, depth + 1, 1);
                game->board[i] = empty;
                best = min(best, value);
            }
        }
        if (best == INT_MAX) {
            return empty;
        }
        return best;
    }
}

const int moves[] = {4, 0, 2, 6, 8, 1, 3, 5, 7};


void main(void) {
    game game;
    initialize(&game, p1);
    printf("Input (-1) for quit\n\n");
    while (1) {
        printf("Make move (1...9): ");
        int inputMove;
        scanf("%d", &inputMove);
        if (inputMove == -1) {
            printf("You quit\n");
            return;
        }
        printf("\n");
        makeMove(&game, inputMove-1);
        int bestScore = 0;
        int bestMove = -1;
        int lastLegalMove = 0;
        for (int i = 0; i < 9; i++) {
            int move = moves[i]+1;
            if (game.board[move] == empty) {
                game.board[move] = p2;
                int value = miniMax(&game, 10, 1);
                game.board[move] = empty;
                lastLegalMove = move;
                if (value <= bestScore) {
                    bestScore = value;
                    bestMove = move;
                }
            }
        }
        if (bestMove == -1) {
            bestMove = lastLegalMove;
        }
        makeMove(&game, bestMove);
        print(&game);
        if (checkWin(&game) == p1) {
            printf("\nYou win\n");
            initialize(&game, p1);
        }
        if (checkWin(&game) == p2) {
            printf("\nAI wins\n");
            initialize(&game, p1);
        }
        int count = 0;
        for (int i = 0; i < 9; i++) {
            if (game.board[i] != empty) {
                count++;
            }
        }
        if (count == 9) {
            printf("\nIts a draw\n");
            initialize(&game, p1);
        }

    }


    printf("%d \n\n", miniMax(&game, 10, 1));
    print(&game);
}

