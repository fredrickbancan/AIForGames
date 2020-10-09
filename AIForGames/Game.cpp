#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#include "Game.h"
Game* Game::gameInstance = nullptr;
#include "TicksAndFPS.h"
#include "EntityLiving.h"
#include "Player.h"
#include <iostream>
#include <random>

/*called every tick*/
void onTick();

Game* Game::get()
{
    if (gameInstance == nullptr)
    {
        gameInstance = new Game();
    }
    return gameInstance;
}

void Game::close()
{
    if (gameInstance != nullptr)
    {
        gameInstance->closeRaylib();
        delete gameInstance;
    }
}

Game::Game()
{
    ticksAndFps = new TicksAndFPS(30);
    thePlayer = new Player(10, 10, 0);
    guards = new EntityLiving[guardCount];
    for (int i = 0; i < guardCount; i++)
    {
        
    }
    InitWindow(screenWidth, screenHeight, "Fredrick - AI for games");
}

Game::~Game()
{
    delete thePlayer;
    delete[] guards;
    delete ticksAndFps;
}

void Game::onFrame()
{
    if (closing = WindowShouldClose()) return;
    ticksAndFps->doOnTickUntillRealtimeSync(onTick);
}

void onTick()
{
    std::cout << "Tick!" << std::endl;
    //TODO: impliment
}

void Game::drawScene()
{
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    DrawText("Hello World", 325, 10, 20, BLACK);

    //TODO: impliment

    EndDrawing();
}

float Game::lerp(float start, float dest)
{
    return start + (dest - start) * ticksAndFps->getPercentageToNextTick();
}

void Game::toggleBooleanOnButtonPress(bool button, bool& booleanToToggle)
{
    if (button)
    {
        booleanToToggle = !booleanToToggle;
    }
}

void Game::closeRaylib()
{
    CloseWindow();        // Close window and OpenGL context
}
