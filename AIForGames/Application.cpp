#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include <iostream>
#include <string>
#include <random>



/*Toggles the provided boolean reference if the provided button bool is true.*/
void toggleBooleanOnButtonPress(bool button, bool& booleanToToggle);

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;


    //Booleans for detecting button presses

    //bool for detecting value box editing

    //int for holding input values


    InitWindow(screenWidth, screenHeight, "Fredrick - Double Linked List (Integers)");
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update

        //Update button booleans


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        DrawText("Double Linked List", 325, 10, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
   //  system("PAUSE");
    return 0;
}

void toggleBooleanOnButtonPress(bool button, bool& booleanToToggle)
{
    if (button)
    {
        booleanToToggle = !booleanToToggle;
    }
}