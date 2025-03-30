#include <emscripten/emscripten.h>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;
Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
const int move_amount = 5.0f;


void UpdateDrawFrame(void);     // Update and Draw one frame

int main(void)
{
        InitWindow(screenWidth, screenHeight, "Hello app.");
        emscripten_set_main_loop(UpdateDrawFrame, 10, 1);
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

