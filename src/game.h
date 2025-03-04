#include "raylib.h"
#include <vector>
#include <random>
#include <ctime>
#include <string>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int BOARD_SIZE = 10;
int CELL_SIZE = 500 / BOARD_SIZE;
int NUM_MINES = 15;
const int WIN_LENGTH = 5;
const int MARKER_PADDING = 5;

const Color PLAYER1_COLOR = BLUE;
const Color PLAYER2_COLOR = ORANGE;
const Color HIGHLIGHT_COLOR = YELLOW;

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    bool isPlayer1 = false;
    bool hasPlayer = false;
    int adjacentMines = 0;
    //animation
    bool isWinningCell = false;
    bool isAnimatingWin = false;
    double animatingWinStartTime = 0;
};

class Game {
private:
    std::vector<std::vector<Cell>> board;
    bool player1Turn;
    bool gameOver;
    std::string winner;
    int player1Score = 0;
    int player2Score = 0;
    int revealedMines = 0;
    
    int CountAdjacentMines(int row, int col) {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int newRow = row + i;
                int newCol = col + j;
                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                    if (board[newRow][newCol].isMine) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
    
    int CheckLineLength(int row, int col, bool isPlayer1) {
        int totolPoints = 0;

        // Define the four directions: horizontal, vertical, and two diagonals
        const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

        // Check each direction
        for (const auto& dir : directions) {
            int count = 1;  // Start with the current cell

            std::vector<std::pair<int, int>> winningCells;
            winningCells.push_back({row, col});
        
            // Check in the positive direction
            int r = row + dir[0], c = col + dir[1];
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE 
                && board[r][c].hasPlayer && board[r][c].isPlayer1 == isPlayer1) 
            {
                count++;
                winningCells.push_back({r, c});
                r += dir[0];
                c += dir[1];
            }

            // Check in the negative direction
            r = row - dir[0], c = col - dir[1];
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE 
                && board[r][c].hasPlayer && board[r][c].isPlayer1 == isPlayer1) 
            {
                count++;
                winningCells.push_back({r, c});
                r -= dir[0];
                c -= dir[1];
            }

            totolPoints += CalculateScore(count);
            for (const auto& cell : winningCells) {
                board[cell.first][cell.second].isWinningCell = true;
                board[cell.first][cell.second].isAnimatingWin = true;
                board[cell.first][cell.second].animatingWinStartTime = GetTime();
            }
        }
        return totolPoints;
    }

    
    int CalculateScore(int length) {
        if (length >= 5) return 3;
        if (length == 4) return 2;
        if (length == 3) return 1;
        return 0;
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
        player1Score = 0;
        player2Score = 0;
        revealedMines = 0;
    }

    void InitializeBoard() {

        board = std::vector<std::vector<Cell>>(BOARD_SIZE, 
               std::vector<Cell>(BOARD_SIZE));
        
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
        
        // Calculate and Display surrounded mines
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (!board[i][j].isMine) {
                    board[i][j].adjacentMines = CountAdjacentMines(i, j);
                }
            }
        }
    }
    
    void ToggleFlag(int row, int col) {
        if (!gameOver && !board[row][col].isRevealed) {
            board[row][col].isFlagged = !board[row][col].isFlagged;
        }
    }
    
    void HandleClick(int row, int col,Sound Boom) {
        if (gameOver || row >= BOARD_SIZE || col >= BOARD_SIZE || 
            board[row][col].isRevealed || board[row][col].isFlagged) {
            return;
        }
        
        board[row][col].isRevealed = true;
        
        if (board[row][col].isMine) {
            PlaySound(Boom);
            revealedMines++;
            // -1 point
            if (player1Turn) {
                player1Score /= 2;
                player1Turn = !player1Turn;
            } else {
                player2Score /= 2;
                player1Turn = !player1Turn;
            }
            // Game ends when all mines are revealed
            if (revealedMines == NUM_MINES) {
                gameOver = true;
                winner = (player1Score > player2Score) ? "Blue" : (player2Score > player1Score) ? "Orange" : "Tie";
            }
            return;
        }
        
        board[row][col].hasPlayer = true;
        board[row][col].isPlayer1 = player1Turn;
        
        int points = CheckLineLength(row, col, player1Turn);

        if (player1Turn) {
            player1Score += points;
        } else {
            player2Score += points;
        }
        
        player1Turn = !player1Turn;

        gameOver = true;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (!board[i][j].isMine && !board[i][j].isRevealed) gameOver = false;
            }
        }
        winner = (player1Score > player2Score) ? "Blue" : (player2Score > player1Score) ? "Orange" : "Tie";

    }
                                
    Color sand = {217, 191, 166, 255};
    Color sandRevealed = {176, 152, 130, 255};
    
    void Draw(Texture2D Bomb,Texture2D BLWin,Texture2D ORWin,Texture2D Flag, Texture2D TIE) {
        // Draw board
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                Rectangle cell = {
                    (float)(SCREEN_WIDTH/2 - (BOARD_SIZE*CELL_SIZE)/2 + j*CELL_SIZE),
                    (float)(SCREEN_HEIGHT/2 - (BOARD_SIZE*CELL_SIZE)/2 + i*CELL_SIZE),
                    (float)CELL_SIZE,
                    (float)CELL_SIZE
                };

                //animation winning cell
                if (board[i][j].isWinningCell) {
                    double elapsedWin = GetTime() - board[i][j].animatingWinStartTime;
                    float scale = 1.0f;
                    if (elapsedWin < 1) {
                        scale = 1.0f + sinf((float)elapsedWin * (float)PI) * 0.2f;
                        cell.x -= (CELL_SIZE * (scale - 1.0f)) / 1;
                        cell.y -= (CELL_SIZE * (scale - 1.0f)) / 2;
                        cell.width *= scale;
                        cell.height *= scale;
                    } else {
                        board[i][j].isAnimatingWin = false;
                    }
                }
                
                if (board[i][j].isRevealed) {
                    DrawRectangleRec(cell, sandRevealed);
                    
                    if (board[i][j].isMine) {
                        DrawTexture(Bomb,cell.x + (CELL_SIZE - Bomb.width) / 2,cell.y + (CELL_SIZE - Bomb.height) / 2,WHITE);
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
                            const char* charCount = count.c_str();
                            Vector2 textSize = MeasureTextEx(GetFontDefault(), charCount, 200 / BOARD_SIZE, 0);
                            DrawText(count.c_str(), 
                                   (int)(cell.x + (CELL_SIZE - textSize.x) / 2),
                                   (int)(cell.y + (CELL_SIZE - textSize.y) / 2),
                                   200 / BOARD_SIZE, WHITE);
                        }
                    }
                } else {
                    DrawRectangleRec(cell, sand);
                    if (board[i][j].isFlagged) {
                        DrawTexture(Flag,cell.x + (CELL_SIZE - Bomb.width) / 2,cell.y + (CELL_SIZE - Bomb.height) / 2,WHITE);
                    }
                }
                DrawRectangleLinesEx(cell, 2, sandRevealed);
            }
        }

        // Draw scores and status
        DrawText(("Blue: " + std::to_string(player1Score)).c_str(), 50, 40, 20, BLUE);
        DrawText(("Orange: " + std::to_string(player2Score)).c_str(), 50, 70, 20, ORANGE);
        DrawText(("Mines Found: " + std::to_string(revealedMines) + "/" + std::to_string(NUM_MINES)).c_str(), 50, 100, 20, RED);
        
        // Draw game status
        std::string status;
        if (gameOver) { 
            if(winner == "Tie"){ DrawTexture(TIE,0,0,WHITE); };
            if(winner == "Blue") DrawTexture(BLWin,0,0,WHITE);
            if(winner == "Orange") DrawTexture(ORWin,0,0,WHITE);
        } else {
            status = "Current Player: " + std::string(player1Turn ? "Blue" : "Orange");
            DrawRectangle(640, 50, 40, 40, (player1Turn ? PLAYER1_COLOR : PLAYER2_COLOR));
        }
        const char* charStatus = status.c_str();
        Vector2 statusSize = MeasureTextEx(GetFontDefault(), charStatus, 20, 0);
        DrawText(status.c_str(),(SCREEN_WIDTH - statusSize.x) / 2 , 10, 20, WHITE);

    }
    
    bool IsGameOver() const { return gameOver; }
};