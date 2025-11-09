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
enum phisicShape
{
    CIRCLE,
    HALFSPACE,
};

class PhysicObject
{
public:

	bool isStatic = false;
    Vector2 position = { 0,0 };
    Vector2 velocity = { 0,0 };
    float mass = 1;

    //float radius = 15;
    std::string name = "object";
    Color color = RED;
	virtual void draw()
	{
		DrawCircle(position.x, position.y, 2, color);
		DrawText(name.c_str(), position.x, position.y, 12, LIGHTGRAY);
		
	}

	virtual phisicShape Shape() = 0;
	
  
};

/*class PhysicBox : public PhysicObject
{
   // Vector2 position = { 0,0 };
    //Vector2 velocity = { 0,0 };
    //float mass = 1;

	//float Hight ;
	//float height ;
    //std::string name = "object";
   
    //Color color = RED;
public:
	Vector2 size ;

	void draw()
	{
		DrawRectangleV(position, size, color);
		DrawText(name.c_str(), position.x, position.y, 12, LIGHTGRAY);
		DrawLineEx(position, position + velocity, 1, color);
	}
};*/


class PhysicCircle : public PhysicObject
{
public:
	float radius;
    Color originalColor = GREEN;
    void draw() override
    {
        DrawCircle(position.x, position.y, radius, color);

        DrawText(name.c_str(), position.x, position.y, radius * 2, LIGHTGRAY);

        DrawLineEx(position, position + velocity, 1, color);
    }

    phisicShape Shape() override
    {
        return CIRCLE;
    }
};

class PhysicHalfSpace : public PhysicObject
{
private:
        float rotation =0;
        Vector2 normal = {0,-1 };
public:
	void setRotationDegrees(float RotationInDegrees)
	{
		rotation = RotationInDegrees;
		normal = Vector2Rotate({ 0,-1 },rotation * DEG2RAD);
	}
    float getRoatation()
    {
		return rotation;

    }
    Vector2 getNormal()
    {
		return normal;
        
    }


		void draw() override
		{
            DrawCircle(position.x, position.y, 8, color);
            DrawLineEx(position, position + velocity, 1, color);

            //Surfface
			Vector2 parallelToSurface = Vector2Rotate(normal, PI * 0.5f);
			DrawLineEx(position - parallelToSurface * 3000, position + parallelToSurface * 3000, 1, color);
		}

		phisicShape Shape() override
		{
			return HALFSPACE;
		}

		
};

bool circleOverlaps(PhysicCircle*circleA, PhysicCircle*circleB)
{
	Vector2 displacementFromAToB = circleB->position - circleA->position;
	float distance = Vector2Length(displacementFromAToB);
	float sumOfRadii = circleA->radius + circleB->radius;
	float overlap = sumOfRadii - distance;
	//Vector2 normalAtoB = displacementFromAToB / distance;
	//Vector2 mtv = normalAtoB * overlap;

	if (sumOfRadii> distance)
	{
		return true;
	}
    else
		return false;

   
}

bool circleColisionResponse(PhysicCircle* circleA, PhysicCircle* circleB)
{
    Vector2 displacementFromAToB = circleB->position - circleA->position;
    float distance = Vector2Length(displacementFromAToB);
    float sumOfRadii = circleA->radius + circleB->radius;
    float overlap = sumOfRadii - distance;

    if (overlap > 0)
    {
    Vector2 normalAtoB = displacementFromAToB / distance;
    Vector2 mtv = normalAtoB * overlap;
        circleA->position -= mtv * 0.5f;
        circleB->position += mtv * 0.5f;

        return true;
    }
    else
        return false;


}



bool circleHalfSpaceOverlaps(PhysicCircle* circle, PhysicHalfSpace* halfspace)
{
	Vector2 displacementFromHSPosToCircle = circle->position - halfspace->position;
	float distance = Vector2DotProduct(displacementFromHSPosToCircle, halfspace->getNormal());

	DrawLineEx(circle->position, halfspace->position, 1, RED);
	Vector2 midpoint = halfspace->position + displacementFromHSPosToCircle * 0.5f ;

	DrawText(TextFormat("D: %6.0f", distance), midpoint.x, midpoint.y, 20, LIGHTGRAY);


    return distance < circle->radius;

}

bool circleHalfspaceCollisionResponse(PhysicCircle* circle, PhysicHalfSpace* halfspace)
{
    Vector2 displacementFromHSPosToCircle = circle->position - halfspace->position;
    float distance = Vector2DotProduct(displacementFromHSPosToCircle, halfspace->getNormal());
    Vector2 projectionDisplacementOntoNormal = halfspace->getNormal() * distance;
    float overlap = circle->radius - distance;

    if (overlap > 0)
    {
        Vector2 midpoint = halfspace->getNormal() * overlap;
        circle->position += midpoint;
        return true;
    }
    else
    {
        return false;
    }
    DrawLineEx(circle->position, halfspace->position, 1, RED);
    Vector2 midpoint = halfspace->position + displacementFromHSPosToCircle * 0.5f;

    DrawText(TextFormat("D: %6.0f", distance), midpoint.x, midpoint.y, 20, LIGHTGRAY);


    return distance < circle->radius;

}


class PhysicWorld
{
private:
    unsigned int ObjectCount = 0;
public:
    std::vector<PhysicObject*> Object; // All objects in physics simulation
    //  std::vector<PhysicBox*> Boxes;
    Vector2 accelerationGravity = { 0, 9 };

    void add(PhysicObject* newObject) // Add to physics simulation
    {
        newObject->name = std::to_string(ObjectCount);
        Object.push_back(newObject);
        ObjectCount++;
    }


    void update()
    {
        for (int i = 0; i < Object.size(); i++)
        {
            PhysicObject* obj = Object[i];

            if (obj->isStatic) continue;


            obj->position = obj->position + obj->velocity * dt;


            obj->velocity = obj->velocity + accelerationGravity * dt;
        }
        checkCollisions();
    }

    void checkCollisions()
    {
		for (int i = 0; i < Object.size(); i++)
        {
			Object[i]->color = GREEN;
            
        }


        for (int i = 0; i < Object.size(); i++)
        {
            for (int j = i + 1; j < Object.size(); j++)
            {
                PhysicObject* objectpointerA = Object[i];
                PhysicObject* objectpointerB = Object[j];

                phisicShape shapeA = objectpointerA->Shape();
                phisicShape shapeB = objectpointerB->Shape();


				bool didCollide = false;


                if (shapeA == CIRCLE && shapeB == CIRCLE)
                {
					didCollide = circleColisionResponse((PhysicCircle*)objectpointerA, (PhysicCircle*)objectpointerB);
                    
                    
                }
                else if (shapeA == CIRCLE && shapeB == HALFSPACE)
                {
                   didCollide = circleHalfspaceCollisionResponse((PhysicCircle*)objectpointerA, (PhysicHalfSpace*)objectpointerB);
                }
                else if (shapeA == HALFSPACE && shapeB == CIRCLE)
                {
				  didCollide = circleHalfspaceCollisionResponse((PhysicCircle*)objectpointerB, (PhysicHalfSpace*)objectpointerA);
                }
                if (didCollide)
                {
                    objectpointerA->color = RED;
                    objectpointerB->color = RED;
                }
            }
        }
    }
};



		//Reset all colors
      /*  for (auto& obj : Object) {
            PhysicCircle* circle = dynamic_cast<PhysicCircle*>(obj);
            if (circle) circle->color = circle->originalColor;
        }
		//changing color if overlapping
        for (int i = 0; i < Object.size(); i++)
        {
            PhysicCircle* circlePointerA = dynamic_cast<PhysicCircle*>(Object[i]);
            if (!circlePointerA) continue;




            for (int j = i + 1; j < Object.size(); j++)
            {
                PhysicCircle* circlePointerB = dynamic_cast<PhysicCircle*>(Object[j]);
                if (!circlePointerB) continue;

                if (circleOverlaps(circlePointerA, circlePointerB))
                {
                    circlePointerA->color = RED;
                    circlePointerB->color = RED;
                }

                
            }

            //if (circleHalfSpaceOverlaps(circlePointerA, &halfspace))
            //{
              //  circlePointerA->color = RED;
            //}*/
        //}
        
   // }
//};
PhysicWorld world;
PhysicHalfSpace halfspace;
PhysicHalfSpace halfspace2;
//Remove objects offscreen
void cleanup()
{
    // checking if the object is offscreen!
    for (int i = 0; i < world.Object.size(); i++)
    {
        PhysicObject* obj = world.Object[i];

        //Is it offscreen?
        if (  obj->position.y > GetScreenHeight()
            || obj->position.y < 0
            || obj->position.x > GetScreenWidth()
            || obj->position.x < 0
            )
        {
            //Destroy!
			auto iterator = world.Object.begin() + i;
			PhysicObject* pointerToPhysicObject = *iterator;
			delete pointerToPhysicObject;
            world.Object.erase(iterator);
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
		PhysicCircle* newBird = new PhysicCircle();//new kryword
        newBird->position = launchPosition;
		newBird->velocity = { launchSpeed * (float)cos(launchAngle * DEG2RAD), -launchSpeed * (float)sin(launchAngle * DEG2RAD) };
		
		
		newBird->radius = (rand() % 26) + 5; 
		//Color randomColor = {rand() % 256 , rand() % 256, rand() % 256, 255};
		//newBird->color = randomColor;
        newBird->color = GREEN;
        newBird->originalColor = GREEN;

		world.add(newBird);
	}
}

void Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Amir Abolhassani 101541841", 10, float(GetScreenHeight() - 30), 20, LIGHTGRAY);

    
    GuiSliderBar(Rectangle{ 70, 5, 200, 10 }, "Launch X", TextFormat("%.0f", launchPosition.x), &launchPosition.x, 0, GetScreenWidth());
    GuiSliderBar(Rectangle{ 70, 25, 200, 10 }, "Launch Y", TextFormat("%.0f", launchPosition.y), &launchPosition.y, 0, GetScreenHeight());
    GuiSliderBar(Rectangle{ 70, 45, 200, 10 }, "Angle", TextFormat("%.1f", launchAngle), &launchAngle, 0, 360);
    GuiSliderBar(Rectangle{ 70, 65, 200, 10 }, "Speed", TextFormat("%.1f", launchSpeed), &launchSpeed, 0, 1000);
    GuiSliderBar(Rectangle{ 70,85,200, 10}, "Gravity Y", TextFormat("Gravity Y: %.0f Px/sec^2", world.accelerationGravity.y), &world.accelerationGravity.y, -1000, 1000);

    DrawText(TextFormat("Object: %i", world.Object.size()), 500,20, 50, LIGHTGRAY);

    DrawText(TextFormat("T: %6.2f", time), GetScreenWidth() - 140, 10, 30, LIGHTGRAY);

    Vector2 velocityVec = { launchSpeed * cos(launchAngle * DEG2RAD), -launchSpeed * sin(launchAngle * DEG2RAD) };
    DrawLineEx(launchPosition, Vector2Add(launchPosition, velocityVec), 3, RED);

	//Halfspace surface  
    GuiSliderBar(Rectangle{ 70, 105, 200, 10 }, "HalfSpace x", TextFormat("%.0f", halfspace.position.x), &halfspace.position.x, 0, GetScreenWidth());
    GuiSliderBar(Rectangle{ 70, 125, 200, 10 }, "HalfSpace Y", TextFormat("%.0f", halfspace.position.y), &halfspace.position.y, 0, GetScreenHeight());

	float halfSpaceRoatation = halfspace.getRoatation();
    GuiSliderBar(Rectangle{ 70, 145, 200, 10 }, "HS Roatition", TextFormat("%.0f", halfspace.getRoatation()), &halfSpaceRoatation, -360,360 );

	halfspace.setRotationDegrees(halfSpaceRoatation);

    //Second halfspace controls
    GuiSliderBar(Rectangle{ 70, 165, 200, 10 }, "HalfSpace2 x", TextFormat("%.0f", halfspace2.position.x), &halfspace2.position.x, 0, GetScreenWidth());
    GuiSliderBar(Rectangle{ 70, 185, 200, 10 }, "HalfSpace2 Y", TextFormat("%.0f", halfspace2.position.y), &halfspace2.position.y, 0, GetScreenHeight());

    float halfSpace2Roatation = halfspace2.getRoatation();
    GuiSliderBar(Rectangle{ 70, 205, 200, 10 }, "HS2 Roatition", TextFormat("%.0f", halfspace2.getRoatation()), &halfSpace2Roatation, -360, 360);

    halfspace2.setRotationDegrees(halfSpace2Roatation);

    //Draw all physics objects!
    for (int i = 0; i < world.Object.size(); i++)
    {
        world.Object[i]->draw();
    }
	halfspace.draw();

    EndDrawing();

}

int main()
{
    InitWindow(InitialWidth, InitialHeight, "Angry bird beta, Amir Abolhassani 101541841");
    SetTargetFPS(TARGET_FPS);
	halfspace.isStatic = true;
	halfspace.position = { 500, 700 };
	halfspace.setRotationDegrees(10);
	world.add(&halfspace);
	halfspace2.isStatic = true;
	halfspace2.position = { 800, 400 };
	halfspace2.setRotationDegrees(-10);
	world.add(&halfspace2);

    while (!WindowShouldClose()) // Loops TARGET_FPS times per second
    {
        update();
        Draw();
    }

    CloseWindow();
    return 0;
}
