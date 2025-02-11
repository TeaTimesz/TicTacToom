#include "game.h"

int main() {
    
    InitWindow(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE + 40, "Minesweeper");
    SetTargetFPS(60);
    
    Game game;
    
    while (!WindowShouldClose()) {
        game.Update();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}