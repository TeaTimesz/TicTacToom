#include "game.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic-Tac-Toom");
    SetTargetFPS(60);
    
    Game game;
    
    while (!WindowShouldClose()) {
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
        {
            Vector2 mousePos = GetMousePosition();
            int boardX = (mousePos.x - (SCREEN_WIDTH/2 - (BOARD_SIZE*CELL_SIZE)/2)) / CELL_SIZE;
            int boardY = (mousePos.y - (SCREEN_HEIGHT/2 - (BOARD_SIZE*CELL_SIZE)/2)) / CELL_SIZE;
            
            if (boardX >= 0 && boardX < BOARD_SIZE && boardY >= 0 && boardY < BOARD_SIZE) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    game.HandleClick(boardY, boardX);
                } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    game.ToggleFlag(boardY, boardX);
                }
            }
        }
        
        if (IsKeyPressed(KEY_R)) {
            game.Reset();
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        game.Draw();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}