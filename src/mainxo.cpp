// main.cpp
#include "raylib.h"
#include "xo.h"

int main(void) {
    InitWindow(800, 800, "Tic-Tac-Toe");
    SetTargetFPS(60);

    TicTacToe game;

    while (!WindowShouldClose()) {
        game.UpdateGame();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        game.DrawBoard();
        game.DrawCurrentPlayer();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
