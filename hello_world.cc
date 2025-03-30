#include <emscripten/emscripten.h>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;
Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
const int move_amount = 5.0f;

#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 20
#define SNAKE_SPEED 15

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct {
    int x;
    int y;
} Position;



// Game variables
std::deque<Position> snake = {{5, 5}, {5, 6}, {5, 7}};
Direction direction = UP;
Position food = {10, 10};
bool gameOver = false;
int score = 0;
int framesCounter = 0;

void UpdateDrawFrame(void);     // Update and Draw one frame
void SnekLoop(void);     // Update and Draw one frame

int main(void)
{

// Initialize random seed
srand(time(NULL));
        InitWindow(screenWidth, screenHeight, "Hello app.");
        emscripten_set_main_loop(SnekLoop, 60, 1);
        CloseWindow();        // Close window and OpenGL context
        return 0;
}

void UpdateDrawFrame(void)
{
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello everybody!!", 190, 200, 20, LIGHTGRAY);
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += move_amount;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= move_amount;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= move_amount;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += move_amount;
	DrawCircleV(ballPosition, 50, MAROON);
        DrawFPS(10, 10);

        EndDrawing();
}

void SnekLoop(void) {
    
        // Update
        framesCounter++;
        
        // Controls
        if (IsKeyPressed(KEY_UP) && direction != DOWN) direction = UP;
        if (IsKeyPressed(KEY_DOWN) && direction != UP) direction = DOWN;
        if (IsKeyPressed(KEY_LEFT) && direction != RIGHT) direction = LEFT;
        if (IsKeyPressed(KEY_RIGHT) && direction != LEFT) direction = RIGHT;
        
        // Move snake (based on game speed)
        if (framesCounter >= (60/SNAKE_SPEED) && !gameOver) {
            framesCounter = 0;
            
            // Calculate new head position
            Position newHead = snake.front();
            switch(direction) {
                case UP: newHead.y--; break;
                case DOWN: newHead.y++; break;
                case LEFT: newHead.x--; break;
                case RIGHT: newHead.x++; break;
            }
            
            // Check collision with walls
            if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH/GRID_SIZE || 
                newHead.y < 0 || newHead.y >= SCREEN_HEIGHT/GRID_SIZE) {
                gameOver = true;
            }
            
            // Check collision with self
            for (const auto& segment : snake) {
                if (newHead.x == segment.x && newHead.y == segment.y) {
                    gameOver = true;
                    break;
                }
            }
            
            // Move if not game over
            if (!gameOver) {
                // Add new head
                snake.push_front(newHead);
                
                // Check if food eaten
                if (newHead.x == food.x && newHead.y == food.y) {
                    // Increase score
                    score++;
                    
                    // Generate new food
                    bool validPosition;
                    do {
                        validPosition = true;
                        food.x = GetRandomValue(0, (SCREEN_WIDTH/GRID_SIZE) - 1);
                        food.y = GetRandomValue(0, (SCREEN_HEIGHT/GRID_SIZE) - 1);
                        
                        // Make sure food doesn't spawn on snake
                        for (const auto& segment : snake) {
                            if (food.x == segment.x && food.y == segment.y) {
                                validPosition = false;
                                break;
                            }
                        }
                    } while (!validPosition);
                } else {
                    // Remove tail if no food eaten
                    snake.pop_back();
                }
            }
        }
        
        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            
            // Draw snake
            for (const auto& segment : snake) {
                DrawRectangle(segment.x * GRID_SIZE, segment.y * GRID_SIZE, 
                              GRID_SIZE, GRID_SIZE, GREEN);
            }
            
            // Draw food
            DrawRectangle(food.x * GRID_SIZE, food.y * GRID_SIZE, 
                          GRID_SIZE, GRID_SIZE, RED);
            
            // Draw score
            DrawText(TextFormat("SCORE: %i", score), 10, 10, 20, WHITE);
            
            // Game over message
            if (gameOver) {
                DrawText("GAME OVER", SCREEN_WIDTH/2 - MeasureText("GAME OVER", 40)/2, 
                         SCREEN_HEIGHT/2 - 40, 40, WHITE);
                DrawText("PRESS [ENTER] TO PLAY AGAIN", 
                         SCREEN_WIDTH/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2,
                         SCREEN_HEIGHT/2 + 10, 20, WHITE);
                
                // Restart game
                if (IsKeyPressed(KEY_ENTER)) {
                    snake = {{5, 5}, {5, 6}, {5, 7}};
                    direction = UP;
                    food = {10, 10};
                    gameOver = false;
                    score = 0;
                }
            }
            
        EndDrawing();
}

