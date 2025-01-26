#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Cell {
  Rectangle rect;
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
  SetTargetFPS(10);
  InitWindow(screenDimensions.x, screenDimensions.y, "Conway's Game of Life");

  int gridWidth = (int)floor(screenDimensions.x / 8);
  int gridHeight = (int)floor(screenDimensions.y / 8);

  Cell cells[gridWidth][gridHeight];

  for (int i = 0; i < gridWidth; i++) {
    for (int j = 0; j < gridHeight; j++) {
      cells[i][j].rect = (Rectangle){i * 8, j * 8, 8, 8};
      cells[i][j].alive = false;
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
      }
    } else { // The sim IS running
             // Create tmp variable so all updates happen "at once"
      Cell tmp[gridWidth][gridHeight];
      for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
          tmp[i][j] = cells[i][j];
        }
      }
      for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
          Cell c = cells[i][j];
          int sumLivingNeighbors = 0;

          if (i < gridWidth) {

            if (j < gridHeight && cells[i + 1][j + 1].alive)
              sumLivingNeighbors++;
            if (j > 0 && cells[i + 1][j - 1].alive)
              sumLivingNeighbors++;
            if (cells[i + 1][j].alive)
              sumLivingNeighbors++;
          }
          if (i > 0) {

            if (j < gridHeight && cells[i - 1][j + 1].alive)
              sumLivingNeighbors++;
            if (j > 0 && cells[i - 1][j - 1].alive)
              sumLivingNeighbors++;
            if (cells[i - 1][j].alive)
              sumLivingNeighbors++;
          }
          if (j < gridHeight && cells[i][j + 1].alive)
            sumLivingNeighbors++;
          if (j > 0 && cells[i][j - 1].alive)
            sumLivingNeighbors++;

          if (sumLivingNeighbors < 2) {
            c.alive = false;
          }
          if (sumLivingNeighbors > 3) {
            c.alive = false;
          }
          if (sumLivingNeighbors == 3) {
            c.alive = true;
          }

          tmp[i][j] = c;
        }
      }

      // Set cells to tmp
      for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
          cells[i][j] = tmp[i][j];
        }
      }
    }

    // Toggle sim on/off
    if (IsKeyReleased(KEY_SPACE)) {
      simRunning = !simRunning;
    }

    // Draw
    BeginDrawing();
    // DrawGrid2D(screenDimensions, 8);
    if (simRunning) {
      DrawText("running", 0, 0, 24, BLUE);
    }
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
