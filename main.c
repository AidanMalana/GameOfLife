#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Cell {
  Rectangle rect;
  bool neighbors[8];
  bool alive;
} Cell;

void DrawGrid2D(Vector2 dimensions, int spacing) {
  for (int i = 0; i < dimensions.x / spacing; i++) {
    DrawLine(i * spacing, 0, i * spacing,
             floor(dimensions.y / spacing) * spacing, WHITE);
  }
  for (int i = 0; i < dimensions.y / spacing; i++) {
    DrawLine(0, i * spacing, floor(dimensions.x / spacing) * spacing,
             i * spacing, WHITE);
  }
}

const Vector2 screenDimensions = {640, 360};

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
  InitWindow(screenDimensions.x, screenDimensions.y, "Conway's Game of Life");

  int gridWidth = (int)floor(screenDimensions.x / 8);
  int gridHeight = (int)floor(screenDimensions.y / 8);

  Cell cells[gridWidth][gridHeight];

  for (int i = 0; i < gridWidth; i++) {
    for (int j = 0; j < gridHeight; j++) {
      cells[i][j].rect = (Rectangle){i * 8, j * 8, 8, 8};
      for (int k = 0; k < 9; k++) {
        *(cells[i][j].neighbors + k) = false;
      }
    }
  }

  int simRunning = false;

  while (!WindowShouldClose()) {
    if (!simRunning) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetMousePosition();
        pos.x = floor(pos.x / 8);
        pos.y = floor(pos.y / 8);
        cells[(int)pos.x][(int)pos.y].alive =
            !cells[(int)pos.x][(int)pos.y].alive;
        if (pos.x + 1 < gridWidth) {
          if (pos.y + 1 < gridHeight) {
            cells[(int)pos.x][(int)pos.y].neighbors[0] =
                cells[(int)pos.x][(int)pos.y].alive;
          }
          if (pos.y - 1 > 0) {
          }
        }
        if (pos.x - 1 > 0) {
          if (pos.y + 1 < gridHeight) {
          }
          if (pos.y - 1 > 0) {
          }
        }
        // cells[(int)pos.x][(int)pos.y].neighbors[0] =
        // cells[(int)pos.x][(int)pos.y].alive;
      }
    }
    BeginDrawing();
    DrawGrid2D(screenDimensions, 8);
    for (int i = 0; i < gridWidth; i++) {
      for (int j = 0; j < gridHeight; j++) {
        if (cells[i][j].alive) {
          Rectangle r = cells[i][j].rect;
          DrawRectangle(r.x, r.y, r.width, r.height, WHITE);
        }
      }
    }
    ClearBackground(BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
