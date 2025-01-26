#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Cell {
  Rectangle rect;
  bool alive;
} Cell;

void DrawGrid2D(Vector2 dimensions, int spacing) {
  for (int i = 0; i <= dimensions.x / spacing; i++) {
    DrawLine(i * spacing, 0, i * spacing, dimensions.y, WHITE);
  }
  for (int i = 0; i <= dimensions.y / spacing; i++) {
    DrawLine(0, i * spacing, floor(dimensions.x / spacing) * spacing,
             i * spacing, WHITE);
  }
}

Vector2 screenDimensions = {640, 360};

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(10);
  InitWindow(screenDimensions.x, screenDimensions.y, "Conway's Game of Life");

  double virtRatio = screenDimensions.x / 640 > screenDimensions.y / 360
                         ? screenDimensions.y / 360
                         : screenDimensions.x / 640;
  virtRatio = floor(virtRatio);
  virtRatio = virtRatio < 1 ? 1 : virtRatio;

  Vector2 offset = {screenDimensions.x - (screenDimensions.x * virtRatio),
                    screenDimensions.y - (screenDimensions.y * virtRatio)};
  offset.x = offset.x / 2;
  offset.y = offset.y / 2;

  Camera2D cam = {
      .offset = offset, .target = {0, 0}, .zoom = virtRatio, .rotation = 0};

  int gridWidth = (int)floor(screenDimensions.x / 8);
  int gridHeight = (int)floor(screenDimensions.y / 8);

  bool drawGrid = true;

  Cell cells[gridWidth][gridHeight];

  for (int i = 0; i < gridWidth; i++) {
    for (int j = 0; j < gridHeight; j++) {
      cells[i][j].rect = (Rectangle){i * 8, j * 8, 8, 8};
      cells[i][j].alive = false;
    }
  }

  int simRunning = false;

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      screenDimensions.x = GetScreenWidth();
      screenDimensions.y = GetScreenHeight();
      virtRatio = screenDimensions.x / 640 > screenDimensions.y / 360
                      ? screenDimensions.y / 360
                      : screenDimensions.x / 640;
      virtRatio = floor(virtRatio);
      virtRatio = virtRatio < 1 ? 1 : virtRatio;
      cam.zoom = virtRatio;

      offset = (Vector2){(screenDimensions.x / 2) - (640 * virtRatio / 2),
                         (screenDimensions.y / 2) - (360 * virtRatio / 2)};
      cam.offset.x = offset.x;
      cam.offset.y = offset.y;
    }
    if (!simRunning) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetScreenToWorld2D(GetMousePosition(), cam);
        pos.x = floor(pos.x / 8);
        pos.y = floor(pos.y / 8);
        if (pos.x < 80 && pos.y < 45) {
          cells[(int)pos.x][(int)pos.y].alive =
              !cells[(int)pos.x][(int)pos.y].alive;
        }
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

          if (i + 1 < gridWidth) {

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

    // Toggle grid
    if (IsKeyReleased(KEY_G)) {
      drawGrid = !drawGrid;
    }

    // Clear grid
    if (IsKeyReleased(KEY_C) || IsKeyReleased(KEY_BACKSPACE)) {
      for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
          cells[i][j].alive = false;
        }
      }
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(cam);
    if (drawGrid)
      DrawGrid2D((Vector2){640, 360}, 8);
    if (simRunning) {
      DrawText("Running", 0, 0, 24, BLUE);
    } else {
      DrawText("Editing", 0, 0, 24, RED);
    }
    for (int i = 0; i < gridWidth; i++) {
      for (int j = 0; j < gridHeight; j++) {
        if (cells[i][j].alive) {
          Rectangle r = cells[i][j].rect;
          DrawRectangle(r.x, r.y, r.width, r.height, WHITE);
        }
      }
    }
    EndMode2D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
