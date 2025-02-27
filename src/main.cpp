#include "game.h"
#include "button.h"

Game game;

enum class GameState { MENU, TUTORIAL, MODE_SELECT, CUSTOM_SETUP, GAME};
GameState currentState = GameState::MENU;

void DrawButton(Rectangle button, const char* text, Color color) {
    DrawRectangleRec(button, color);
    int textWidth = MeasureText(text, 20);
    DrawText(text, button.x + (button.width - textWidth) / 2, button.y + (button.height - 20) / 2, 20, BLACK);
}

bool IsButtonClicked(Rectangle button) {
    return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TicTacToom");
    SetTargetFPS(60);

    Texture2D menu_background = LoadTexture("graphics/menubackground.png");
    Texture2D game_background = LoadTexture("graphics/gamebackground.png");
    Texture2D selection_background = LoadTexture("graphics/MenubackgroundNotext.png");
    Texture2D BLWin = LoadTexture("graphics/WinnerBl.png");
    Texture2D ORWin = LoadTexture("graphics/WinnerOr.png");
    Texture2D TextBackground = LoadTexture("graphics/5.png");

    Button startButton("graphics/1.png",{510,300},0.5);
    Button tutorialButton("graphics/2.png",{510,430},0.5);
    Button backButton("graphics/3.png",{510,560},0.5);
    Button exitButton("graphics/7.png",{1000,560},0.5);
    Button defaultButton("graphics/4.png",{510,200},0.5);
    Button customButton ("graphics/6.png",{510,330},0.5);
    Button sizeUp("graphics/8.png",{750,100},0.25);
    Button sizeDown("graphics/9.png",{400,100},0.25);
    Button mineUp("graphics/8.png",{750,230},0.25);
    Button mineDown("graphics/9.png",{400,230},0.25);

    Image image1 = LoadImage("graphics/10.png");
    ImageResize(&image1,CELL_SIZE,CELL_SIZE);
    Texture2D Bomb = LoadTextureFromImage(image1);
    Image image2 = LoadImage("graphics/11.png");
    ImageResize(&image2,CELL_SIZE,CELL_SIZE);
    Texture2D Flag = LoadTextureFromImage(image2);

    while (!WindowShouldClose()) {
        BeginDrawing();

        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        switch (currentState) {
            case GameState::MENU:
                BOARD_SIZE = 10;
                NUM_MINES = 15;
                CELL_SIZE = 500 / BOARD_SIZE;
                game.InitializeBoard();
                ClearBackground(RAYWHITE);
                DrawTexture(menu_background,0,0,WHITE);
            
                startButton.Draw();
                tutorialButton.Draw();
            
                if (startButton.isPressed(mousePosition,mousePressed)) currentState = GameState::MODE_SELECT;
                if (tutorialButton.isPressed(mousePosition,mousePressed)) currentState = GameState::TUTORIAL;
                break;

            case GameState::TUTORIAL:
                ClearBackground(RAYWHITE);
                DrawTexture(selection_background,0,0,WHITE);
                DrawText("How to Play", SCREEN_WIDTH / 2 - 100, 100, 30, DARKGREEN);
                DrawText("1. Left click to reveal a cell.", 100, 200, 20, BLACK);
                DrawText("2. Right click to flag a mine.", 100, 240, 20, BLACK);
                DrawText("3. Connect 3-5 cells in a row for points.", 100, 280, 20, BLACK);
                DrawText("4. Find all mines to end the game.", 100, 320, 20, BLACK);
        
                backButton.Draw();
                if (backButton.isPressed(mousePosition,mousePressed)) currentState = GameState::MENU;
                break;

            case GameState::MODE_SELECT:
                ClearBackground(RAYWHITE);
                DrawTexture(selection_background,0,0,WHITE);
                DrawTexture(TextBackground,480,40,WHITE);
                DrawText("Select Game Mode", SCREEN_WIDTH / 2 - 150, 100, 30, WHITE);

                defaultButton.Draw();
                customButton.Draw();
                backButton.Draw();

                if (defaultButton.isPressed(mousePosition,mousePressed)) {
                    ::BOARD_SIZE = 7;
                    ::CELL_SIZE = 500 / BOARD_SIZE;
                    ::NUM_MINES = 7;
                    currentState = GameState::GAME;
                }
                if (customButton.isPressed(mousePosition,mousePressed)) currentState = GameState::CUSTOM_SETUP;
                if (backButton.isPressed(mousePosition,mousePressed)) currentState = GameState::MENU;
                    break;
                
            case GameState::CUSTOM_SETUP:
                ClearBackground(RAYWHITE);
                DrawTexture(selection_background,0,0,WHITE);
                DrawTexture(TextBackground,480,100,WHITE);
                DrawTexture(TextBackground,480,200,WHITE);
                DrawText(("Board Size: " + std::to_string(BOARD_SIZE)).c_str(), 570, 140, 20, WHITE);
                DrawText(("Number of Mines: " + std::to_string(NUM_MINES)).c_str(), 540, 270, 20, WHITE);
                
                sizeUp.Draw();
                sizeDown.Draw();
                mineUp.Draw();
                mineDown.Draw();
                startButton.Draw();
                backButton.Draw();

                if (sizeUp.isPressed(mousePosition,mousePressed) && BOARD_SIZE< 10) { 
                    BOARD_SIZE++; 
                    CELL_SIZE = 500 / BOARD_SIZE; 
                }
                if (sizeDown.isPressed(mousePosition,mousePressed) && BOARD_SIZE > 5) { 
                    BOARD_SIZE--; 
                    CELL_SIZE = 500 / BOARD_SIZE;
                }
                if (mineUp.isPressed(mousePosition,mousePressed) && NUM_MINES < BOARD_SIZE * BOARD_SIZE / 2) NUM_MINES++;
                if (mineDown.isPressed(mousePosition,mousePressed) && NUM_MINES > 1) NUM_MINES--;
        
                if (startButton.isPressed(mousePosition,mousePressed)) { 
                    game.InitializeBoard();
                    currentState = GameState::GAME;
                }
                if (backButton.isPressed(mousePosition,mousePressed)) currentState = GameState::MODE_SELECT;
                break;
                
            case GameState::GAME:
                ClearBackground(RAYWHITE);
                DrawTexture(game_background, 0, 0, WHITE);
                DrawTexture(TextBackground, 510, 0, WHITE);
                DrawTexture(TextBackground, 0, 0, WHITE);
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
                
                game.Draw(Bomb,BLWin,ORWin,Flag);

                //back button
                exitButton.Draw();
                if (exitButton.isPressed(mousePosition,mousePressed)) {
                    currentState = GameState::MENU;
                    game.Reset();
                }
                break;
        
        }
        EndDrawing();
    }
    UnloadImage(image1);
    CloseWindow();
    return 0;
}
