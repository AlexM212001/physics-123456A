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

// Launch 
Vector2 launchPosition = { 200, 500 };
float launchAngle = 45.0f; 
float launchSpeed = 100.0f;

Vector2 velocity = { 0, 0 };

void Update()
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

    // Calculate velocity 
    float rad = launchAngle * DEG2RAD;
    velocity.x = cos(rad) * launchSpeed;
    velocity.y = -sin(rad) * launchSpeed; 

 
}

void Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Alpha testing for Angry Bird ", 10, GetScreenHeight() - 30, 20, LIGHTGRAY);

    // GUI controls
    GuiSliderBar(Rectangle{ 60, 5, 200, 10 }, "Launch X", TextFormat("%.0f", launchPosition.x), &launchPosition.x, 0, GetScreenWidth());
    GuiSliderBar(Rectangle{ 60, 25, 200, 10 }, "Launch Y", TextFormat("%.0f", launchPosition.y), &launchPosition.y, 0, GetScreenHeight());
    GuiSliderBar(Rectangle{ 60, 45, 200, 10 }, "Angle", TextFormat("%.1f", launchAngle), &launchAngle, 0, 360);
    GuiSliderBar(Rectangle{ 60, 65, 200, 10 }, "Speed", TextFormat("%.1f", launchSpeed), &launchSpeed, 0, 1000);

    // Draw launch position
    DrawCircleV(launchPosition, 20, PINK);
    float visualScale = 0.2f; // Adjust this value for best visual effect
    Vector2 velocityVector = Vector2Scale(Vector2Normalize(velocity), launchSpeed * visualScale);
    Vector2 endPoint = Vector2Add(launchPosition, velocityVector);
    DrawLineV(launchPosition, endPoint, RED);
    DrawText(TextFormat("Velocity: (%.1f, %.1f)", velocity.x, velocity.y), 60, 90, 20, LIGHTGRAY);

    EndDrawing();
}

int main()
{
    InitWindow(1200, 800, "Alpha testing for Angry Bird");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}
