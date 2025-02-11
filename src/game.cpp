#include "game.h"

Game::Game() {
    InitializeBoard();
    PlaceMines();
    CalculateAdjacentMines();
    gameOver = false;
    gameWon = false;
}

void Game::InitializeBoard() {
    board.resize(BOARD_SIZE, std::vector<Cell>(BOARD_SIZE));
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].rect = {
                static_cast<float>(j * CELL_SIZE),
                static_cast<float>(i * CELL_SIZE),
                static_cast<float>(CELL_SIZE),
                static_cast<float>(CELL_SIZE)
            };
        }
    }
}

void Game::PlaceMines() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, BOARD_SIZE - 1);
    
    int placedMines = 0;
    while (placedMines < MINE_COUNT) {
        int x = dis(gen);
        int y = dis(gen);
        if (!board[y][x].isMine) {
            board[y][x].isMine = true;
            placedMines++;
        }
    }
}

void Game::CalculateAdjacentMines() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!board[i][j].isMine) {
                int count = 0;

                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int ny = i + dy;
                        int nx = j + dx;
                        if (ny >= 0 && ny < BOARD_SIZE && nx >= 0 && nx < BOARD_SIZE) {
                            if (board[ny][nx].isMine) count++;
                        }
                    }
                }
                board[i][j].adjacentMines = count;
            }
        }
    }
}

void Game::FloodFill(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) return;
    if (board[y][x].isRevealed || board[y][x].isFlagged) return;
    
    board[y][x].isRevealed = true;
    
    /*if (board[y][x].adjacentMines == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                FloodFill(x + dx, y + dy);
            }
        }
    }*/
}

void Game::RevealCell(int x, int y) {
    if (board[y][x].isFlagged) return;
    
    if (board[y][x].isMine) {
        gameOver = true;
        return;
    }
    
    FloodFill(x, y);
    gameWon = CheckWinCondition();
}

bool Game::CheckWinCondition() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!board[i][j].isMine && !board[i][j].isRevealed) {
                return false;
            }
        }
    }
    return true;
}

void Game::Update() {
    if (gameOver || gameWon) return;
    
    Vector2 mousePos = GetMousePosition();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (CheckCollisionPointRec(mousePos, board[i][j].rect)) {
                    RevealCell(j, i);
                }
            }
        }
    }
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (CheckCollisionPointRec(mousePos, board[i][j].rect)) {
                    if (!board[i][j].isRevealed) {
                        board[i][j].isFlagged = !board[i][j].isFlagged;
                    }
                }
            }
        }
    }
}

void Game::Draw() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Color cellColor = LIGHTGRAY;
            if (board[i][j].isRevealed) {
                cellColor = RAYWHITE;
            }
            
            DrawRectangleRec(board[i][j].rect, cellColor);
            DrawRectangleLinesEx(board[i][j].rect, 1, DARKGRAY);
            
            if (board[i][j].isRevealed) {
                if (board[i][j].isMine) {
                    DrawText("*", j * CELL_SIZE + 10, i * CELL_SIZE + 8, 20, RED);
                } else if (board[i][j].adjacentMines > 0) {
                    DrawText(TextFormat("%d", board[i][j].adjacentMines),
                            j * CELL_SIZE + 10, i * CELL_SIZE + 8, 20, DARKBLUE);
                }
            } else if (board[i][j].isFlagged) {
                DrawText("F", j * CELL_SIZE + 10, i * CELL_SIZE + 8, 20, RED);
            }
        }
    }
    
    if (gameOver) {
        DrawText("Game Over!", 10, BOARD_SIZE * CELL_SIZE + 10, 20, RED);
    } else if (gameWon) {
        DrawText("You Win!", 10, BOARD_SIZE * CELL_SIZE + 10, 20, GREEN);
    }
}