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

// Tutorial images
std::vector<Texture2D> tutorialImages;
int currentTutorialImage = 0;

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

    if (!tutorialImages.empty()) {
        int imageWidth = tutorialImages[currentTutorialImage].width;
        int imageHeight = tutorialImages[currentTutorialImage].height;
        
        // คำนวณตำแหน่งให้อยู่กลางจอ
        int posX = (SCREEN_WIDTH - imageWidth) / 2;
        int posY = (SCREEN_HEIGHT - imageHeight) / 2;

        DrawTexture(tutorialImages[currentTutorialImage], posX, posY, WHITE);
    }

    Rectangle backButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };
    Rectangle nextButton = { SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT - 100, 100, 50 };
    Rectangle prevButton = { SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 100, 100, 50 };

    DrawButton(backButton, "Back", RED);
    DrawButton(nextButton, ">", LIGHTGRAY);
    DrawButton(prevButton, "<", LIGHTGRAY);

    if (IsButtonClicked(backButton)) currentState = GameState::MENU;
    if (IsButtonClicked(nextButton) && currentTutorialImage < tutorialImages.size() - 1) currentTutorialImage++;
    if (IsButtonClicked(prevButton) && currentTutorialImage > 0) currentTutorialImage--;
}


void DrawModeSelect() {
    ClearBackground(RAYWHITE);
    DrawText("Select Game Mode", SCREEN_WIDTH / 2 - 150, 100, 30, DARKBLUE);

    Rectangle defaultButton = { SCREEN_WIDTH / 2 - 100, 300, 200, 50 };
    Rectangle customButton = { SCREEN_WIDTH / 2 - 100, 400, 200, 50 };

    DrawButton(defaultButton, "Default Mode", GREEN);
    DrawButton(customButton, "Custom Mode", ORANGE);

    if (IsButtonClicked(defaultButton)) currentState = GameState::GAME;
    if (IsButtonClicked(customButton)) currentState = GameState::CUSTOM_SETUP;
}

void DrawCustomSetup() {
    ClearBackground(RAYWHITE);
    DrawText("Custom Game Setup", SCREEN_WIDTH / 2 - 150, 100, 30, DARKGREEN);

    Rectangle backButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };
    DrawButton(backButton, "Back", RED);

    if (IsButtonClicked(backButton)) currentState = GameState::MODE_SELECT;
}

void LoadTutorialImages() {
    tutorialImages.push_back(LoadTexture("tutorial1.png"));
    tutorialImages.push_back(LoadTexture("tutorial2.png"));
    tutorialImages.push_back(LoadTexture("tutorial3.png"));
    tutorialImages.push_back(LoadTexture("tutorial4.png"));
}

void UnloadTutorialImages() {
    for (Texture2D img : tutorialImages) {
        UnloadTexture(img);
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper Game");
    SetTargetFPS(60);

    LoadTutorialImages();

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
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    Vector2 mousePos = GetMousePosition();
                    int boardX = (mousePos.x - (SCREEN_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2)) / CELL_SIZE;
                    int boardY = (mousePos.y - (SCREEN_HEIGHT / 2 - (BOARD_SIZE * CELL_SIZE) / 2)) / CELL_SIZE;
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

    UnloadTutorialImages();
    CloseWindow();
    return 0;
}
