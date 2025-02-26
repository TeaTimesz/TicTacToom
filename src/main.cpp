#include "button.h"
#include "menu.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper Game");
    SetTargetFPS(60);

    Texture2D menu_background = LoadTexture("graphics/menuBackground.png");
    Texture2D game_background = LoadTexture("graphics/gameBackground.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        switch (currentState) {
            case GameState::MENU:
                BOARD_SIZE = 10;
                NUM_MINES = 15;
                CELL_SIZE = 500 / BOARD_SIZE;
                game.InitializeBoard();
                DrawTexture(menu_background, 0, 0, WHITE);
                DrawMenu();
                break;
            case GameState::TUTORIAL:
                DrawTutorial();
                break;
            case GameState::MODE_SELECT:
                DrawTexture(menu_background, 0, 0, WHITE);
                DrawModeSelect();
                break;
            case GameState::CUSTOM_SETUP:
                DrawCustomSetup();
                break;
                
            case GameState::GAME:
                ClearBackground(RAYWHITE);
                DrawTexture(game_background, 0, 0, WHITE);
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
                
                game.Draw();

                //back button
                Rectangle backButton = { SCREEN_WIDTH - 150, 10, 130, 40 };
                DrawButton(backButton, "Exit Game", RED);
                if (IsButtonClicked(backButton)) {
                    currentState = GameState::MENU;
                    game.Reset();
                }
                break;
        
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
