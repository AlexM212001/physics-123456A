/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"
#include <string>
#include <vector>

const unsigned int TARGET_FPS = 50;
float dt = 1.0f / TARGET_FPS;
float time = 0;

// Launch 
Vector2 launchPosition = { 200, 500 };
float launchAngle = 45.0f; 
float launchSpeed = 300.0f;

Vector2 velocity = { 0, 0 };

void Update()
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

    // Calculate velocity 
  //float rad = launchAngle * DEG2RAD;
  //velocity.x = cos(rad) * launchSpeed;
  //velocity.y = -sin(rad) * launchSpeed; 

 
}
class PhysicObject
{
public:
    Vector2 position = { 0,0 };
    Vector2 velocity = { 0,0 };
    float mass = 1;

    float radius = 15; 
    std::string name = "object";
    Color color = RED;

    void draw()
    {
        DrawCircle(position.x, position.y, radius, color);

        DrawText(name.c_str(), position.x, position.y, radius * 2, LIGHTGRAY);

        DrawLineEx(position, position + velocity, 1, color);
    }
};
class PhysicWorld
{
private:
    unsigned int ObjectCount = 0;
public:
    std::vector<PhysicObject> Object; // All objects in physics simulation
    Vector2 accelerationGravity = { 0, 9 };

    void add(PhysicObject newObject) // Add to physics simulation
    {
        newObject.name = std::to_string(ObjectCount);
        Object.push_back(newObject);
        ObjectCount++;
    }

    
    void update()
    {
        for (int i = 0; i < Object.size(); i++)
        {
            
            Object[i].position = Object[i].position + Object[i].velocity * dt;
           

            Object[i].velocity = Object[i].velocity + accelerationGravity * dt;
        }

    }
};
PhysicWorld world;
//Remove objects offscreen
void cleanup()
{
    // checking if the object is offscreen!
    for (int i = 0; i < world.Object.size(); i++)
    {
        //Is it offscreen?
        if (world.Object[i].position.y > GetScreenHeight()
            || world.Object[i].position.y < 0
            || world.Object[i].position.x > GetScreenWidth()
            || world.Object[i].position.x < 0
            )
        {
            //Destroy!
            world.Object.erase(world.Object.begin() + i);
            i--;
        }
    }

}
void update()
{
	dt = 1.0f / TARGET_FPS;
	time += dt;

	cleanup();
	world.update();

	if (IsKeyPressed(KEY_SPACE))
	{
		PhysicObject newBird;
        newBird.position = launchPosition;
		newBird.velocity = { launchSpeed * (float)cos(launchAngle * DEG2RAD), -launchSpeed * (float)sin(launchAngle * DEG2RAD) };
		
		
		newBird.radius = (rand() % 26) + 5; 
		Color randomColor = {rand() % 256 , rand() % 256, rand() % 256, 255};
		newBird.color = randomColor;

		world.add(newBird);
	}
}

void Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Amir Abolhassani 101541841", 10, float(GetScreenHeight() - 30), 20, LIGHTGRAY);

    
    GuiSliderBar(Rectangle{ 60, 5, 200, 10 }, "Launch X", TextFormat("%.0f", launchPosition.x), &launchPosition.x, 0, GetScreenWidth());
    GuiSliderBar(Rectangle{ 60, 25, 200, 10 }, "Launch Y", TextFormat("%.0f", launchPosition.y), &launchPosition.y, 0, GetScreenHeight());
    GuiSliderBar(Rectangle{ 60, 45, 200, 10 }, "Angle", TextFormat("%.1f", launchAngle), &launchAngle, 0, 360);
    GuiSliderBar(Rectangle{ 60, 65, 200, 10 }, "Speed", TextFormat("%.1f", launchSpeed), &launchSpeed, 0, 1000);
    GuiSliderBar(Rectangle{ 60,85,200, 10}, "Gravity Y", TextFormat("Gravity Y: %.0f Px/sec^2", world.accelerationGravity.y), &world.accelerationGravity.y, -1000, 1000);

    DrawText(TextFormat("Object: %i", world.Object.size()), 10, 160, 30, LIGHTGRAY);

    DrawText(TextFormat("T: %6.2f", time), GetScreenWidth() - 140, 10, 30, LIGHTGRAY);

    Vector2 velocityVec = { launchSpeed * cos(launchAngle * DEG2RAD), -launchSpeed * sin(launchAngle * DEG2RAD) };
    DrawLineEx(launchPosition, Vector2Add(launchPosition, velocityVec), 3, RED);

    //Draw all physics objects!
    for (int i = 0; i < world.Object.size(); i++)
    {
        world.Object[i].draw();
    }

    EndDrawing();

}

int main()
{
    InitWindow(InitialWidth, InitialHeight, "Angry bird beta, Amir Abolhassani 101541841");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose()) // Loops TARGET_FPS times per second
    {
        update();
        Draw();
    }

    CloseWindow();
    return 0;
}
