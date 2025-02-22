#include "raylib.h"
#include <vector>
#include <random>
#include <ctime>
#include <string>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int BOARD_SIZE = 10;
const int CELL_SIZE = 500/BOARD_SIZE;
const int NUM_MINES = 0;
const int WIN_LENGTH = 5;
const Color PLAYER1_COLOR = BLUE;
const Color PLAYER2_COLOR = ORANGE;
const int MARKER_PADDING = 5;

enum class GameMode {
    CLASSIC,
    SCORE
};

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    bool isPlayer1 = false;
    bool hasPlayer = false;
    int adjacentMines = 0;
};

class Game {
private:
    std::vector<std::vector<Cell>> board;
    bool player1Turn;
    bool gameOver;
    std::string winner;
    GameMode currentMode;
    int player1Score = 0;
    int player2Score = 0;
    int revealedMines = 0;
    
    void InitializeBoard() {
        board = std::vector<std::vector<Cell>>(BOARD_SIZE, 
               std::vector<Cell>(BOARD_SIZE));
        
        // Place mines randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, BOARD_SIZE * BOARD_SIZE - 1);
        
        int minesPlaced = 0;
        while (minesPlaced < NUM_MINES) {
            int pos = dis(gen);
            int row = pos / BOARD_SIZE;
            int col = pos % BOARD_SIZE;
            
            if (!board[row][col].isMine) {
                board[row][col].isMine = true;
                minesPlaced++;
            }
        }
        
        // Calculate adjacent mines
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (!board[i][j].isMine) {
                    board[i][j].adjacentMines = CountAdjacentMines(i, j);
                }
            }
        }
    }
    
    int CountAdjacentMines(int row, int col) {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int newRow = row + i;
                int newCol = col + j;
                if (newRow >= 0 && newRow < BOARD_SIZE && 
                    newCol >= 0 && newCol < BOARD_SIZE) {
                    if (board[newRow][newCol].isMine) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
    
    int CheckLineLength(int row, int col, bool isPlayer1) {
    int maxLength = 0;
    // Define the four directions: horizontal, vertical, and two diagonals.
    const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    // Check each direction.
    for (const auto& dir : directions) {
        int count = 1;  // Start with the current cell.
        
        // Check in the positive direction.
        int r = row + dir[0], c = col + dir[1];
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c].hasPlayer && board[r][c].isPlayer1 == isPlayer1) 
        {
            count++;
            r += dir[0];
            c += dir[1];
        }

        // Check in the negative direction.
        r = row - dir[0], c = col - dir[1];
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&board[r][c].hasPlayer && board[r][c].isPlayer1 == isPlayer1) 
        {
            count++;
            r -= dir[0];
            c -= dir[1];
        }

        // Update maxLength if this direction yields a longer line.
        maxLength = std::max(maxLength, count);
    }
    return maxLength;
}

    
    int CalculateScore(int length) {
        if (length >= 5) return 3;
        if (length == 4) return 2;
        if (length == 3) return 1;
        return 0;
    }

public:
    Game(GameMode mode = GameMode::CLASSIC) : currentMode(mode) {
        Reset();
    }
    
    void Reset() {
        InitializeBoard();
        player1Turn = true;
        gameOver = false;
        winner = "";
        player1Score = 0;
        player2Score = 0;
        revealedMines = 0;
    }
    
    void ToggleFlag(int row, int col) {
        if (!gameOver && !board[row][col].isRevealed) {
            board[row][col].isFlagged = !board[row][col].isFlagged;
        }
    }
    
    void HandleClick(int row, int col) {
        if (gameOver || row >= BOARD_SIZE || col >= BOARD_SIZE || 
            board[row][col].isRevealed || board[row][col].isFlagged) {
            return;
        }
        
        board[row][col].isRevealed = true;
        
        if (board[row][col].isMine) {
            revealedMines++;
            if (currentMode == GameMode::CLASSIC) {
                winner = player1Turn ? "Orange" : "Blue";
                gameOver = true;
            } else {
                // Score mode: -1 point for hitting a mine
                if (player1Turn) {
                    player1Score--;
                } else {
                    player2Score--;
                }
                // Game ends when all mines are revealed
                if (revealedMines == NUM_MINES) {
                    gameOver = true;
                    winner = (player1Score > player2Score) ? "Blue" : 
                            (player2Score > player1Score) ? "Orange" : "Tie";
                }
            }
            return;
        }
        
        board[row][col].hasPlayer = true;
        board[row][col].isPlayer1 = player1Turn;
        
        if (currentMode == GameMode::SCORE) {

            int lineLength = CheckLineLength(row, col, player1Turn);
            int points = CalculateScore(lineLength);

            if (player1Turn) {
                player1Score += points;
            } else {
                player2Score += points;
            }  
        } 
        /*else if (CheckLineLength(row, col, player1Turn) >= 3) {
            winner = player1Turn ? "Blue" : "Orange";
            gameOver = true;
            return;
        }*/
        
        player1Turn = !player1Turn;
    }
    
    void Draw() {
        // Draw board
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                Rectangle cell = {
                    (float)(SCREEN_WIDTH/2 - (BOARD_SIZE*CELL_SIZE)/2 + j*CELL_SIZE),
                    (float)(SCREEN_HEIGHT/2 - (BOARD_SIZE*CELL_SIZE)/2 + i*CELL_SIZE),
                    (float)CELL_SIZE,
                    (float)CELL_SIZE
                };
                
                if (board[i][j].isRevealed) {
                    DrawRectangleRec(cell, LIGHTGRAY);
                    
                    if (board[i][j].isMine) {
                        DrawText("*", (int)cell.x + CELL_SIZE/3, 
                               (int)cell.y + CELL_SIZE/3, 40, RED);
                    } else {
                        if (board[i][j].hasPlayer) {
                            Rectangle playerMarker = {
                                cell.x + MARKER_PADDING,
                                cell.y + MARKER_PADDING,
                                cell.width - (MARKER_PADDING * 2),
                                cell.height - (MARKER_PADDING * 2)
                            };
                            DrawRectangleRec(playerMarker, 
                                           board[i][j].isPlayer1 ? PLAYER1_COLOR : PLAYER2_COLOR);
                        }
                        
                        if (board[i][j].adjacentMines > 0) {
                            std::string count = std::to_string(board[i][j].adjacentMines);
                            int textWidth = MeasureText(count.c_str(), 20);
                            DrawText(count.c_str(), 
                                   (int)(cell.x + (CELL_SIZE - textWidth) / 2),
                                   (int)(cell.y + (CELL_SIZE - 20) / 2),
                                   20, WHITE);
                        }
                    }
                } else {
                    DrawRectangleRec(cell, GRAY);
                    if (board[i][j].isFlagged) {
                        DrawText("F", (int)cell.x + CELL_SIZE/3, 
                               (int)cell.y + CELL_SIZE/3, 40, RED);
                    }
                }
                DrawRectangleLinesEx(cell, 2, BLACK);
            }
        }
        
        // Draw scores and status
        if (currentMode == GameMode::SCORE) {
            DrawText(("Blue: " + std::to_string(player1Score)).c_str(), 10, 10, 20, BLUE);
            DrawText(("Orange: " + std::to_string(player2Score)).c_str(), 10, 40, 20, ORANGE);
            DrawText(("Mines Found: " + std::to_string(revealedMines) + "/" + 
                     std::to_string(NUM_MINES)).c_str(), 10, 70, 20, RED);
        }
        
        // Draw game status
        std::string status;
        if (gameOver) {
            if (winner == "Tie") {
                status = "Game Over - It's a Tie!";
            } else {
                status = "Winner: " + winner;
            }
            DrawText("Press R to restart", SCREEN_WIDTH - 200, 10, 20, BLACK);
        } else {
            status = "Current Player: " + std::string(player1Turn ? "Blue" : "Orange");
        }
        DrawText(status.c_str(), SCREEN_WIDTH/2 - 100, 10, 20, BLACK);
    }
    
    bool IsGameOver() const { return gameOver; }
    GameMode GetMode() const { return currentMode; }
    void SetMode(GameMode mode) { 
        currentMode = mode; 
        Reset();
    }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper Tic-tac-toe");
    SetTargetFPS(60);
    
    Game game(GameMode::SCORE);  // เริ่มเกมในโหมดสะสมคะแนน
    
    while (!WindowShouldClose()) {
        // Update
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || 
            IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            Vector2 mousePos = GetMousePosition();
            int boardX = (mousePos.x - (SCREEN_WIDTH/2 - (BOARD_SIZE*CELL_SIZE)/2)) / CELL_SIZE;
            int boardY = (mousePos.y - (SCREEN_HEIGHT/2 - (BOARD_SIZE*CELL_SIZE)/2)) / CELL_SIZE;
            
            if (boardX >= 0 && boardX < BOARD_SIZE && 
                boardY >= 0 && boardY < BOARD_SIZE) {
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
        
        if (IsKeyPressed(KEY_M)) {  // สลับโหมดเกม
            if (game.GetMode() == GameMode::CLASSIC) {
                game.SetMode(GameMode::SCORE);
            } else {
                game.SetMode(GameMode::CLASSIC);
            }
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        game.Draw();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}