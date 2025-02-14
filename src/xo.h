///
#ifndef XO_H
#define XO_H

#include "raylib.h"

#define BOARDSIZE 3
#define CELLSIZE 200

typedef enum { EMPTY, X, O } Player;

class TicTacToe {
    private:
        bool trunx;
        Player board[BOARDSIZE][BOARDSIZE];

    public:
        TicTacToe();
        void DrawBoard();
        void UpdateGame();
        void DrawCurrentPlayer();
};

#endif
