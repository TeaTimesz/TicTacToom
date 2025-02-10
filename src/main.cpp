#include "raylib.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream> // Include for string stream

// Define constants for grid size and number of mines
const int gridWidth = 10;
const int gridHeight = 10;
const int totalMines = 15;
const int cellSize = 40;

// Cell state constants
enum CellState { HIDDEN, REVEALED, FLAGGED };

// Structure for each grid cell
struct Cell {
    CellState state;
    bool isMine;
    int surroundingMines;
};

std::vector<std::vector<Cell>> grid(gridWidth, std::vector<Cell>(gridHeight));

void InitializeGrid() {
    // Initialize all cells
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            grid[i][j].state = HIDDEN;
            grid[i][j].isMine = false;
            grid[i][j].surroundingMines = 0;
        }
    }

    // Randomly place mines
    int minesPlaced = 0;
    while (minesPlaced < totalMines) {
        int x = rand() % gridWidth;
        int y = rand() % gridHeight;
        if (!grid[x][y].isMine) {
            grid[x][y].isMine = true;
            minesPlaced++;

            // Update surrounding cells with mine count
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int nx = x + i;
                    int ny = y + j;
                    if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                        grid[nx][ny].surroundingMines++;
                    }
                }
            }
        }
    }
}

std::string IntToString(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

void DrawGrid() {
    // Draw all cells
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            Rectangle cellRect = { (float)(i * cellSize), (float)(j * cellSize), (float)cellSize, (float)cellSize };

            // Color for the cell
            Color cellColor = (grid[i][j].state == HIDDEN) ? LIGHTGRAY : (grid[i][j].isMine ? RED : BEIGE);
            DrawRectangleRec(cellRect, cellColor);

            // Draw the number of surrounding mines if revealed
            if (grid[i][j].state == REVEALED && grid[i][j].surroundingMines > 0) {
                Color numberColor = (grid[i][j].surroundingMines == 1) ? BLUE :
                                    (grid[i][j].surroundingMines == 2) ? GREEN :
                                    (grid[i][j].surroundingMines == 3) ? DARKBLUE : DARKGRAY;
                DrawText(IntToString(grid[i][j].surroundingMines).c_str(), (i * cellSize) + 12, (j * cellSize) + 12, 20, numberColor);
            }

            // Draw a flag if the cell is flagged
            if (grid[i][j].state == FLAGGED) {
                DrawText("F", (i * cellSize) + 12, (j * cellSize) + 12, 20, DARKGRAY);
            }
        }
    }
}

void RevealCell(int x, int y) {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return;
    if (grid[x][y].state == REVEALED || grid[x][y].state == FLAGGED) return;

    grid[x][y].state = REVEALED;

    // If no surrounding mines, reveal adjacent cells
    if (grid[x][y].surroundingMines == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                RevealCell(x + i, y + j);
            }
        }
    }
}

int main() {
    // Initialize window
    InitWindow(400, 400, "Minesweeper");
    SetTargetFPS(60);

    // Seed random number generator
    srand(time(0));

    // Initialize game grid
    InitializeGrid();

    // Game loop
    while (!WindowShouldClose()) {
        // Get mouse position
        Vector2 mousePos = GetMousePosition();

        // Get grid cell where mouse is hovering
        int mouseX = mousePos.x / cellSize;
        int mouseY = mousePos.y / cellSize;

        // Process mouse input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Reveal cell on left click
            if (mouseX >= 0 && mouseX < gridWidth && mouseY >= 0 && mouseY < gridHeight) {
                RevealCell(mouseX, mouseY);
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            // Toggle flag on right click
            if (mouseX >= 0 && mouseX < gridWidth && mouseY >= 0 && mouseY < gridHeight) {
                if (grid[mouseX][mouseY].state == HIDDEN) {
                    grid[mouseX][mouseY].state = FLAGGED;
                } else if (grid[mouseX][mouseY].state == FLAGGED) {
                    grid[mouseX][mouseY].state = HIDDEN;
                }
            }
        }

        // Draw everything
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGrid();
        EndDrawing();
    }

    // Close window
    CloseWindow();

    return 0;
}
