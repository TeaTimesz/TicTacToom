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
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D menu_background = LoadTexture("graphics/menubackground.png");
    Texture2D game_background = LoadTexture("graphics/gamebackground.png");
    Texture2D selection_background = LoadTexture("graphics/MenubackgroundNotext.png");
    Texture2D BLWin = LoadTexture("graphics/WinnerBl.png");
    Texture2D ORWin = LoadTexture("graphics/WinnerOr.png");
    Texture2D TIE = LoadTexture("graphics/draw.png");
    Texture2D TextBackground = LoadTexture("graphics/5.png");
    Texture2D tutorialImage1 = LoadTexture("graphics/Tutorial1.png"); 
    Texture2D tutorialImage2 = LoadTexture("graphics/Tutorial2.png");
    Texture2D tutorialImage3 = LoadTexture("graphics/Tutorial3.png");
    Texture2D tutorialImage4 = LoadTexture("graphics/Tutorial4.png");
    Texture2D nextArrow = LoadTexture("graphics/next.png"); // ลูกศรขวา (ถัดไป)
    Texture2D prevArrow = LoadTexture("graphics/pre.png");
    

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

    Music bgm = LoadMusicStream("sounds/bgm.mp3");
    PlayMusicStream(bgm);
    Sound pop = LoadSound("sounds/pop.mp3");
    Sound boom = LoadSound("sounds/boom.mp3");


    while (!WindowShouldClose()) {
        BeginDrawing();

        UpdateMusicStream(bgm);

        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        switch (currentState) {
            case GameState::MENU:
                BOARD_SIZE = 7;
                NUM_MINES = 7;
                CELL_SIZE = 500 / BOARD_SIZE;
                game.InitializeBoard();
                ClearBackground(RAYWHITE);
                DrawTexture(menu_background,0,0,WHITE);
            
                startButton.Draw();
                tutorialButton.Draw();
            
                if (startButton.isPressed(mousePosition,mousePressed)) {currentState = GameState::MODE_SELECT;PlaySound(pop);}
                if (tutorialButton.isPressed(mousePosition,mousePressed)) {currentState = GameState::TUTORIAL;PlaySound(pop);}
                break;

                case GameState::TUTORIAL: {
                    ClearBackground(RAYWHITE);
                    DrawTexture(selection_background, 0, 0, WHITE);
                
                    static int currentImageIndex = 0; 
                    Texture2D tutorialImages[] = { tutorialImage1, tutorialImage2, tutorialImage3, tutorialImage4 }; // อาร์เรย์รูปภาพ
                
                    Texture2D currentImage = tutorialImages[currentImageIndex];
                
                    float scale = 0.82;
                    Vector2 imageSize = { (float)currentImage.width * scale, (float)currentImage.height * scale };
                
                    Vector2 imagePosition = {
                        (SCREEN_WIDTH - imageSize.x) / 2, // ตำแหน่ง X (อยู่ตรงกลาง)
                        (SCREEN_HEIGHT - imageSize.y) / 2 - 20 // ตำแหน่ง Y (ขยับขึ้นไปข้างบน 20 พิกเซล)
                    };
                
                    // วาดรูปภาพ
                    DrawTextureEx(currentImage, imagePosition, 0, scale, WHITE);
                
                    // โหลดรูปภาพลูกศร
                     // ลูกศรซ้าย (ย้อนกลับ)
                
                    // ปรับขนาดลูกศร
                    float arrowScale = 0.30; // ปรับขนาดลูกศรให้เล็กลง 
                
                    // ปุ่มลูกศรซ้าย (ย้อนกลับ)
                    if (currentImageIndex > 0) {
                        Vector2 nextArrowPosition = {
                            50, // ตำแหน่ง X (ด้านซ้ายของหน้าจอ)
                            SCREEN_HEIGHT / 2 - nextArrow.height * arrowScale / 2 // ตำแหน่ง Y (กึ่งกลางในแนวตั้ง)
                        };
                        DrawTextureEx(nextArrow, nextArrowPosition, 0, arrowScale, WHITE);
                        if (CheckCollisionPointRec(mousePosition, { nextArrowPosition.x, nextArrowPosition.y, (float)nextArrow.width * arrowScale, (float)nextArrow.height * arrowScale }) && mousePressed) {
                            currentImageIndex--; // เลื่อนไปรูปภาพก่อนหน้า
                        }
                    }
                
                    //ปุ่มลูกศรขวา (ถัดไป)
                    if (currentImageIndex < 3) {
                        Vector2 prevArrowPosition = {
                            SCREEN_WIDTH - 200, // ตำแหน่ง X (ด้านขวาของหน้าจอ)
                            SCREEN_HEIGHT / 2 - prevArrow.height * arrowScale / 2 // ตำแหน่ง Y (กึ่งกลางในแนวตั้ง)
                        };
                        DrawTextureEx(prevArrow, prevArrowPosition, 0, arrowScale, WHITE);
                        if (CheckCollisionPointRec(mousePosition, { prevArrowPosition.x, prevArrowPosition.y, (float)prevArrow.width * arrowScale, (float)prevArrow.height * arrowScale }) && mousePressed) {
                            currentImageIndex++; // เลื่อนไปรูปภาพถัดไป
                        }
                    }
                
                    backButton.Draw();
                    if (backButton.isPressed(mousePosition, mousePressed)) {
                        PlaySound(pop);
                        currentState = GameState::MENU;
                    }
                    break;
                }
            case GameState::MODE_SELECT:
                ClearBackground(RAYWHITE);
                DrawTexture(selection_background,0,0,WHITE);
                DrawTexture(TextBackground,480,40,WHITE);
                DrawText("Select Game Mode", SCREEN_WIDTH / 2 - 150, 100, 30, WHITE);

                defaultButton.Draw();
                customButton.Draw();
                backButton.Draw();

                if (defaultButton.isPressed(mousePosition,mousePressed)) {
                    PlaySound(pop);
                    ::BOARD_SIZE = 7;
                    ::CELL_SIZE = 500 / BOARD_SIZE;
                    ::NUM_MINES = 7;
                    currentState = GameState::GAME;
                }
                if (customButton.isPressed(mousePosition,mousePressed)) {currentState = GameState::CUSTOM_SETUP;PlaySound(pop);}
                if (backButton.isPressed(mousePosition,mousePressed)) {currentState = GameState::MENU;PlaySound(pop);}
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
                    PlaySound(pop);
                    BOARD_SIZE++; 
                    CELL_SIZE = 500 / BOARD_SIZE; 
                }
                if (sizeDown.isPressed(mousePosition,mousePressed) && BOARD_SIZE > 5) {
                    PlaySound(pop);
                    BOARD_SIZE--; 
                    CELL_SIZE = 500 / BOARD_SIZE;
                }
                if (mineUp.isPressed(mousePosition,mousePressed) && NUM_MINES < BOARD_SIZE * BOARD_SIZE / 2) {NUM_MINES++;PlaySound(pop);}
                if (mineDown.isPressed(mousePosition,mousePressed) && NUM_MINES > 1) {NUM_MINES--;PlaySound(pop);}
        
                if (startButton.isPressed(mousePosition,mousePressed)) { 
                    PlaySound(pop);
                    game.InitializeBoard();
                    currentState = GameState::GAME;
                }
                if (backButton.isPressed(mousePosition,mousePressed)) {currentState = GameState::MODE_SELECT;PlaySound(pop);}
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
                            game.HandleClick(boardY, boardX,boom);
                        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                            game.ToggleFlag(boardY, boardX);
                        }
                    }
                }
                
                if (IsKeyPressed(KEY_R)) {
                    game.Reset();
                }
                
                game.Draw(Bomb,BLWin,ORWin,Flag,TIE);
                exitButton.Draw();
                if (exitButton.isPressed(mousePosition,mousePressed)) {
                    PlaySound(pop);
                    currentState = GameState::MENU;
                    game.Reset();
                }
                break;
        
        }
        EndDrawing();

    }
    CloseAudioDevice();
    UnloadImage(image1);
    UnloadImage(image2);
    CloseWindow();
    return 0;
}