#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include <vector>
#include <random>

const int BOARD_SIZE = 10;
const int CELL_SIZE = 32;
const int MINE_COUNT = 15;

class Cell {
public:
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
    Rectangle rect;
    
    Cell() : isMine(false), isRevealed(false), isFlagged(false), adjacentMines(0) {}
};

class Game {
private:
    
    std::vector<std::vector<Cell>> board;
    bool gameOver;
    bool gameWon;
    
    void InitializeBoard();
    void PlaceMines();
    void CalculateAdjacentMines();
    void RevealCell(int x, int y);
    void FloodFill(int x, int y);
    bool CheckWinCondition();
    
public:
    Game();
    void Update();
    void Draw();
    bool IsGameOver() const { return gameOver; }
    bool IsGameWon() const { return gameWon; }
};

#endif