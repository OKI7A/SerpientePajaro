#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define CELL_SIZE 32
#define GRID_WIDTH  20
#define GRID_HEIGHT 15
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)

typedef struct {
    int x, y;
} Vector2i;
// pollos es el mejor del universo si o yeah yeeeeeeetttt 4 letters one word geee ge yeet
typedef struct {
    int x, y, w, h;
} Platform;

// -----------------------------------------------------
// FUNCIONES AUXILIARES
// -----------------------------------------------------
bool CellInsideGrid(Vector2i c) {
    return c.x >= 0 && c.x < GRID_WIDTH && c.y >= 0 && c.y < GRID_HEIGHT;
}

bool CollidesWithPlatform(Vector2i pos, Platform *plats, int count) {
    for (int i = 0; i < count; i++) {
        if (pos.x >= plats[i].x && pos.x < plats[i].x + plats[i].w &&
            pos.y >= plats[i].y && pos.y < plats[i].y + plats[i].h)
            return true;
    }
    return false;
}

bool IsOnGround(Vector2i *snake, int len, Platform *plats, int count) {
    for (int i = 0; i < len; i++) {
        Vector2i below = { snake[i].x, snake[i].y + 1 };
        if (below.y >= GRID_HEIGHT) return true; // suelo
        if (CollidesWithPlatform(below, plats, count)) return true;
        // parte del cuerpo debajo
        for (int j = 0; j < len; j++) {
            if (i != j && snake[j].x == below.x && snake[j].y == below.y)
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------
// PROGRAMA PRINCIPAL
// -----------------------------------------------------
int main(void) {
    InitWindow(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, "Snakebird");
    SetTargetFPS(60);

    // --- Plataformas ---
    Platform platforms[3] = {
        {0, GRID_HEIGHT - 1, GRID_WIDTH, 1},    // suelo
        {5, 10, 5, 1},                         // plataforma media
        {12, 7, 3, 1}                          // plataforma alta
    };
    int platformCount = 3;

    // --- Serpiente ---
    Vector2i snake[MAX_SNAKE_LENGTH];
    int snakeLength = 4;
    snake[0] = (Vector2i){5, 5};
    snake[1] = (Vector2i){5, 6};
    snake[2] = (Vector2i){5, 7};
    snake[3] = (Vector2i){5, 8};

    // --- Variables ---
    bool moved = false;
    float gravityTimer = 0;
    const float gravityDelay = 0.25f; // caída más lenta

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        gravityTimer += dt;
        moved = false;

        // --- Movimiento manual (por tecla) ---
        Vector2i dir = {0, 0};
        if (IsKeyPressed(KEY_RIGHT)) { dir = (Vector2i){1, 0}; moved = true; }
        if (IsKeyPressed(KEY_LEFT))  { dir = (Vector2i){-1, 0}; moved = true; }
        if (IsKeyPressed(KEY_UP))    { dir = (Vector2i){0, -1}; moved = true; }
        if (IsKeyPressed(KEY_DOWN))  { dir = (Vector2i){0, 1}; moved = true; }

        if (moved) {
            Vector2i newHead = {snake[0].x + dir.x, snake[0].y + dir.y};

            // Validar que no salga ni atraviese plataformas
            if (CellInsideGrid(newHead) && !CollidesWithPlatform(newHead, platforms, platformCount)) {
                // Mover cuerpo
                for (int i = snakeLength - 1; i > 0; i--)
                    snake[i] = snake[i - 1];
                snake[0] = newHead;
            }
        }

        // --- Gravedad ---
        if (gravityTimer >= gravityDelay) {
            gravityTimer = 0;
            if (!IsOnGround(snake, snakeLength, platforms, platformCount)) {
                // Caer todo el cuerpo
                for (int i = 0; i < snakeLength; i++) {
                    Vector2i below = {snake[i].x, snake[i].y + 1};
                    if (!CollidesWithPlatform(below, platforms, platformCount) && below.y < GRID_HEIGHT)
                        snake[i].y += 1;
                }
            }
        }

        // --- DIBUJO ----
        BeginDrawing();
        ClearBackground((Color){25, 25, 40, 255});

        // Dibujar cuadrícula
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, (Color){255, 255, 255, 25});
            }
        }

        // Dibujar plataformas
        for (int i = 0; i < platformCount; i++) {
            DrawRectangle(platforms[i].x * CELL_SIZE, platforms[i].y * CELL_SIZE,
                          platforms[i].w * CELL_SIZE, platforms[i].h * CELL_SIZE, BROWN);
        }

        // Dibujar serpiente
        for (int i = 0; i < snakeLength; i++) {
            Color c = (i == 0) ? GREEN : LIME;
            DrawRectangle(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, c);
        }

        DrawText("Snakebird by Pollos y Peibol", 10, 10, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}