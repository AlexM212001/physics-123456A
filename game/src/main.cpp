/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const unsigned int TARGET_FPS = 50;
float dt = 1.0f / TARGET_FPS;
float time = 0;
float x = 500;
float y = 500;
float frequency = 1.0f;
float amplitude = 100.0f;

void Update()
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

	x = x + (-sin(time * frequency)) * frequency * amplitude * dt;
    y = y + (cos (time * frequency)) * frequency * amplitude * dt;

}
void Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("game 2005 101541841 Amir", 10, float(GetScreenHeight() - 30), 20, LIGHTGRAY);

    GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);
    DrawText("Time", GetScreenWidth() - 150, 10, 30, LIGHTGRAY);

	DrawCircle(x, y, 70, RED);
	DrawCircle(500 + cos(time * frequency) * amplitude, 500 + sin(time * frequency) * amplitude, 70, BLUE);

    EndDrawing();

}




int main()
{
    InitWindow(1200, 800, "game 2005 101541841 Amir ");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
		Update();
		Draw();   
    }

    CloseWindow();
    return 0;
}
