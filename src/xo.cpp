//โค้ด
#include "xo.h"

TicTacToe::TicTacToe() {
    trunx = false;
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

void TicTacToe::DrawBoard() {
    int offX = (GetScreenWidth() - BOARDSIZE * CELLSIZE) / 2;
    int offY = (GetScreenHeight() - BOARDSIZE * CELLSIZE) / 2;

    for (int i = 0; i <= BOARDSIZE; i++) {
        DrawLine(offX, offY + i * CELLSIZE, offX + BOARDSIZE * CELLSIZE, offY + i * CELLSIZE, DARKGRAY);
        DrawLine(offX + i * CELLSIZE, offY, offX + i * CELLSIZE, offY + BOARDSIZE * CELLSIZE, DARKGRAY);
    }

    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (board[i][j] == X) {
                DrawText("X", offX + j * CELLSIZE + CELLSIZE / 4, offY + i * CELLSIZE + CELLSIZE / 5, 160, RED);
            } else if (board[i][j] == O) {
                DrawText("O", offX + j * CELLSIZE + CELLSIZE / 4, offY + i * CELLSIZE + CELLSIZE / 6, 160, BLUE);
            }
        }
    }
}

void TicTacToe::UpdateGame() {
    static Player currentPlayer = X;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        int offX = (GetScreenWidth() - BOARDSIZE * CELLSIZE) / 2;
        int offY = (GetScreenHeight() - BOARDSIZE * CELLSIZE) / 2;

        if (mouseX >= offX && mouseX < offX + BOARDSIZE * CELLSIZE &&
            mouseY >= offY && mouseY < offY + BOARDSIZE * CELLSIZE) {

            int row = (mouseY - offY) / CELLSIZE;
            int col = (mouseX - offX) / CELLSIZE;

            if (row >= 0 && row < BOARDSIZE && col >= 0 && col < BOARDSIZE && board[row][col] == EMPTY) {
                board[row][col] = currentPlayer;

                currentPlayer = (currentPlayer == X) ? O : X;
                trunx = (currentPlayer == X);
            }
        }
    }
}

void TicTacToe::DrawCurrentPlayer() {
    if (trunx) {
        DrawText("Player X's turn", 2, 20, 20, BLACK);
    } else {
        DrawText("Player O's turn", 2, 20, 20, BLACK);
    }
}