#include "raylib.h"
#include <vector>
#include <random>
#include <ctime>
#include <string>

const int SCREEN_WIDTH = 1280; //1280;//1920;//1024;
const int SCREEN_HEIGHT = 720; //720;//1080;//768;
const int CELL_SIZE = 50;
const int BOARD_SIZE = 10;
const int NUM_MINES = 20;
const int WIN_LENGTH = 3;

const Color PLAYER1_COLOR = BLUE;  // player 1
const Color PLAYER2_COLOR = ORANGE;  // player 2

const int MARKER_PADDING = 5;  // ระยะห่างขอบของสีในช่อง

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isPlayer1 = false;  // แทน player 1 ด้วย true (BLUE)
    bool hasPlayer = false;
    int adjacentMines = 0;
};

class Game {

private:
    std::vector<std::vector<Cell>> board;
    bool player1Turn;  // true = player1 , false = player2
    bool gameOver;
    std::string winner;
    
    void InitializeBoard() {
        board = std::vector<std::vector<Cell>>(BOARD_SIZE, 
               std::vector<Cell>(BOARD_SIZE));
        
        //rand() % 100
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
    
    bool CheckWin(int row, int col, bool isPlayer1) {
        // Check horizontal
        for (int j = 0; j <= BOARD_SIZE - WIN_LENGTH; j++) {
            int count = 0;
            for (int k = 0; k < WIN_LENGTH; k++) {
                if (board[row][j + k].hasPlayer && 
                    board[row][j + k].isPlayer1 == isPlayer1) {
                    count++;
                }
            }
            if (count == WIN_LENGTH) return true;
        }
        
        // Check vertical
        for (int i = 0; i <= BOARD_SIZE - WIN_LENGTH; i++) {
            int count = 0;
            for (int k = 0; k < WIN_LENGTH; k++) {
                if (board[i + k][col].hasPlayer && 
                    board[i + k][col].isPlayer1 == isPlayer1) {
                    count++;
                }
            }
            if (count == WIN_LENGTH) return true;
        }
        
        // Check diagonal (top-left to bottom-right)
        for (int i = 0; i <= BOARD_SIZE - WIN_LENGTH; i++) {
            for (int j = 0; j <= BOARD_SIZE - WIN_LENGTH; j++) {
                int count = 0;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (board[i + k][j + k].hasPlayer && 
                        board[i + k][j + k].isPlayer1 == isPlayer1) {
                        count++;
                    }
                }
                if (count == WIN_LENGTH) return true;
            }
        }
        
        // Check diagonal (top-right to bottom-left)
        for (int i = 0; i <= BOARD_SIZE - WIN_LENGTH; i++) {
            for (int j = WIN_LENGTH - 1; j < BOARD_SIZE; j++) {
                int count = 0;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (board[i + k][j - k].hasPlayer && 
                        board[i + k][j - k].isPlayer1 == isPlayer1) {
                        count++;
                    }
                }
                if (count == WIN_LENGTH) return true;
            }
        }
        
        return false;
    }

public:
    Game() {
        Reset();
    }
    
    void Reset() {
        InitializeBoard();
        player1Turn = true;
        gameOver = false;
        winner = "";
    }
    
    void HandleClick(int row, int col) {
        if (gameOver || row >= BOARD_SIZE || col >= BOARD_SIZE || 
            board[row][col].isRevealed) {
            return;
        }
        
        board[row][col].isRevealed = true;
        
        if (board[row][col].isMine) {
            winner = player1Turn ? "Orange" : "Blue";
            gameOver = true;
            return;
        }
        
        board[row][col].hasPlayer = true;
        board[row][col].isPlayer1 = player1Turn;
        
        if (CheckWin(row, col, player1Turn)) {
            winner = player1Turn ? "Blue" : "Orange";
            gameOver = true;
            return;
        }
        
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
                        // วาดสีของผู้เล่น
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
                        
                        // วาดจำนวนระเบิดตรงกลาง
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
                }
                DrawRectangleLinesEx(cell, 2, BLACK);
            }
        }
        
        // Draw game status
        std::string status;
        if (gameOver) {
            status = "Winner: " + winner;
        } else {
            status = "Current Player: " + std::string(player1Turn ? "Blue" : "Orange");
            DrawRectangle(25, 50, 80, 80, player1Turn ? BLUE : ORANGE);
        }
        DrawText(status.c_str(), 10, 10, 20, BLACK);
        
        if (gameOver) {
            DrawText("Press R to restart", 10, 40, 20, BLACK);
            
        }
    }
    
    bool IsGameOver() const { return gameOver; } // getter function เผื่อเอาไปใช้ต่อ
};