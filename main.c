#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 20
#define GRID_WIDTH 40
#define GRID_HEIGHT 30
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)

typedef struct {
    int x, y;
} Vector2i;

int main() {
    InitWindow(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, "Snake by pollos y peibol");
    SetTargetFPS(60);

    Vector2i snake[MAX_SNAKE_LENGTH];
    int snakeLength = 3;
    snake[0] = (Vector2i){GRID_WIDTH / 2, GRID_HEIGHT / 2};
    snake[1] = (Vector2i){snake[0].x - 1, snake[0].y};
    snake[2] = (Vector2i){snake[1].x - 1, snake[1].y};

    Vector2i direction = {0, 0}; // No se mueve al inicio
    Vector2i food;
    bool foodExists = false;
    bool gameOver = false;

    srand(time(NULL));

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Detectar dirección y mover solo cuando se presione una tecla
            bool moved = false;
            Vector2i newDir = direction;

            if (IsKeyPressed(KEY_UP) && direction.y == 0) { newDir = (Vector2i){0, -1}; moved = true; }
            if (IsKeyPressed(KEY_DOWN) && direction.y == 0) { newDir = (Vector2i){0, 1}; moved = true; }
            if (IsKeyPressed(KEY_LEFT) && direction.x == 0) { newDir = (Vector2i){-1, 0}; moved = true; }
            if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) { newDir = (Vector2i){1, 0}; moved = true; }

            if (moved || (direction.x != 0 || direction.y != 0)) {
                // Solo mover si se presionó una tecla o ya hay dirección activa
                direction = newDir;

                // Generar comida si no existe
                if (!foodExists) {
                    food.x = rand() % GRID_WIDTH;
                    food.y = rand() % GRID_HEIGHT;
                    foodExists = true;
                }

                // Si se presionó una tecla de movimiento, mover la serpiente una celda
                if (moved) {
                    for (int i = snakeLength - 1; i > 0; i--) {
                        snake[i] = snake[i - 1];
                    }

                    snake[0].x += direction.x;
                    snake[0].y += direction.y;

                    // Comprobar colisiones con bordes
                    if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH ||
                        snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
                        gameOver = true;
                    }

                    // Comprobar colisiones con el cuerpo
                    for (int i = 1; i < snakeLength; i++) {
                        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                            gameOver = true;
                            break;
                        }
                    }

                    // Comer comida
                    if (snake[0].x == food.x && snake[0].y == food.y) {
                        if (snakeLength < MAX_SNAKE_LENGTH) {
                            snake[snakeLength] = snake[snakeLength - 1];
                            snakeLength++;
                        }
                        foodExists = false;
                    }
                }
            }
        } else {
            // Reiniciar juego
            if (IsKeyPressed(KEY_R)) {
                snakeLength = 3;
                snake[0] = (Vector2i){GRID_WIDTH / 2, GRID_HEIGHT / 2};
                snake[1] = (Vector2i){snake[0].x - 1, snake[0].y};
                snake[2] = (Vector2i){snake[1].x - 1, snake[1].y};
                direction = (Vector2i){0, 0};
                foodExists = false;
                gameOver = false;
            }
        }

        // --- Dibujo ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (!gameOver) {
            // Dibuja comida
            if (foodExists)
                DrawRectangle(food.x * CELL_SIZE, food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);

            // Dibuja serpiente
            for (int i = 0; i < snakeLength; i++) {
                Color color = (i == 0) ? GREEN : LIME;
                DrawRectangle(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, color);
            }

          
        } else {
            DrawText("GAME OVER!", 300, 260, 40, RED);
            DrawText("Pressiona R", 270, 310, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}