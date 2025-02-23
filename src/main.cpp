#include "raylib.h"
#include "game.h"
#include <vector>
#include <random>
#include <ctime>
#include <string>

Game game;

// Game states
enum class GameState { MENU, TUTORIAL, MODE_SELECT, CUSTOM_SETUP, GAME};
GameState currentState = GameState::MENU;

// Button helper functions
void DrawButton(Rectangle button, const char* text, Color color) {
    DrawRectangleRec(button, color);
    int textWidth = MeasureText(text, 20);
    DrawText(text, button.x + (button.width - textWidth) / 2, button.y + (button.height - 20) / 2, 20, BLACK);
}

bool IsButtonClicked(Rectangle button) {
    return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// Menu Screen
void DrawMenu() {
    ClearBackground(RAYWHITE);
    DrawText("Tic-Tac-Toom", SCREEN_WIDTH / 2 - 150, 100, 40, DARKBLUE);

    Rectangle startButton = { SCREEN_WIDTH / 2 - 100, 300, 200, 50 };
    Rectangle tutorialButton = { SCREEN_WIDTH / 2 - 100, 400, 200, 50 };

    DrawButton(startButton, "Start", GREEN);
    DrawButton(tutorialButton, "Tutorial", SKYBLUE);

    if (IsButtonClicked(startButton)) currentState = GameState::MODE_SELECT;
    if (IsButtonClicked(tutorialButton)) currentState = GameState::TUTORIAL;
}

void DrawTutorial() {
    ClearBackground(RAYWHITE);
    DrawText("How to Play", SCREEN_WIDTH / 2 - 100, 100, 30, DARKGREEN);
    DrawText("1. Left click to reveal a cell.", 100, 200, 20, BLACK);
    DrawText("2. Right click to flag a mine.", 100, 240, 20, BLACK);
    DrawText("3. Connect 3-5 cells in a row for points.", 100, 280, 20, BLACK);
    DrawText("4. Find all mines to end the game.", 100, 320, 20, BLACK);

    Rectangle backButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };
    DrawButton(backButton, "Back", RED);
    if (IsButtonClicked(backButton)) currentState = GameState::MENU;
}

void DrawModeSelect() {
    ClearBackground(RAYWHITE);
    DrawText("Select Game Mode", SCREEN_WIDTH / 2 - 150, 100, 30, DARKBLUE);

    Rectangle defaultButton = { SCREEN_WIDTH / 2 - 100, 300, 200, 50 };
    Rectangle customButton = { SCREEN_WIDTH / 2 - 100, 400, 200, 50 };
    Rectangle backButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };

    DrawButton(defaultButton, "Default", GREEN);
    DrawButton(customButton, "Custom", ORANGE);
    DrawButton(backButton, "Back", RED);

    if (IsButtonClicked(defaultButton)) {
        ::BOARD_SIZE = 10;
        ::NUM_MINES = 15;
        currentState = GameState::GAME;
    }
    if (IsButtonClicked(customButton)) currentState = GameState::CUSTOM_SETUP;
    if (IsButtonClicked(backButton)) currentState = GameState::MENU;
}

void DrawCustomSetup() {
    ClearBackground(RAYWHITE);
    DrawText("Custom Game Setup", SCREEN_WIDTH / 2 - 150, 100, 30, DARKGREEN);

    DrawText(("Board Size: " + std::to_string(BOARD_SIZE)).c_str(), 400, 250, 20, BLACK);
    DrawText(("Number of Mines: " + std::to_string(NUM_MINES)).c_str(), 400, 300, 20, BLACK);

    Rectangle sizeUp = { 700, 240, 30, 30 };
    Rectangle sizeDown = { 350, 240, 30, 30 };
    Rectangle minesUp = { 700, 290, 30, 30 };
    Rectangle minesDown = { 350, 290, 30, 30 };
    Rectangle startButton = { SCREEN_WIDTH / 2 - 100, 400, 200, 50 };
    Rectangle backButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };

    DrawButton(sizeUp, "+", LIGHTGRAY);
    DrawButton(sizeDown, "-", LIGHTGRAY);
    DrawButton(minesUp, "+", LIGHTGRAY);
    DrawButton(minesDown, "-", LIGHTGRAY);
    DrawButton(startButton, "Start Game", GREEN);
    DrawButton(backButton, "Back", RED);

    if (IsButtonClicked(sizeUp) && BOARD_SIZE < 10) { BOARD_SIZE++; CELL_SIZE = 500 / BOARD_SIZE; }
    if (IsButtonClicked(sizeDown) && BOARD_SIZE > 5) { BOARD_SIZE--; CELL_SIZE = 500 / BOARD_SIZE; }
    if (IsButtonClicked(minesUp) && NUM_MINES < BOARD_SIZE * BOARD_SIZE / 2) NUM_MINES++;
    if (IsButtonClicked(minesDown) && NUM_MINES > 1) NUM_MINES--;

    if (IsButtonClicked(startButton)) { 
        game.InitializeBoard();
        currentState = GameState::GAME;
    }
    if (IsButtonClicked(backButton)) currentState = GameState::MODE_SELECT;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        switch (currentState) {
            case GameState::MENU:
                BOARD_SIZE = 10;
                NUM_MINES = 15;
                CELL_SIZE = 500 / BOARD_SIZE;
                game.InitializeBoard();
                DrawMenu();
                break;
            case GameState::TUTORIAL:
                DrawTutorial();
                break;
            case GameState::MODE_SELECT:
                DrawModeSelect();
                break;
            case GameState::CUSTOM_SETUP:
                DrawCustomSetup();
                break;
                
            case GameState::GAME:
                ClearBackground(RAYWHITE);
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
