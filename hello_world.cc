#include <emscripten/emscripten.h>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;

void UpdateDrawFrame(void);     // Update and Draw one frame

int main(void)
{
        InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
        CloseWindow();        // Close window and OpenGL context
        return 0;
}

void UpdateDrawFrame(void)
{
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello everybody!!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
}

