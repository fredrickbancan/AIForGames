#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#include "Game.h"
Game* Game::gameInstance = nullptr;
#include "TicksAndFPS.h"
#include "Player.h"
#include "Guard.h"
#include <iostream>
#include <random>
#include "math.h"

//triangle vertices
const Vector2 triVert0{ 0.0F, -25.0F };
const Vector2 triVert1{ -10.0F, 0.0F };
const Vector2 triVert2{ 10.0F, 0.0F };

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
    guards = new Guard[guardCount];

    for (int i = 0; i < guardCount; i++)
    {
        guards[i].setPos(screenWidth  - (rand() % screenWidth), rand() %  screenHeight);
        guards[i].setRotation(rand() % 360);
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
    ticksAndFps->doOnTickUntillRealtimeSync(this);
}

void Game::onTick()
{
    for (int i = 0; i < guardCount; i++)
    {
        guards[i].onTick();
    }
    thePlayer->onTick();
}

void Game::drawScene()
{
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    DrawText("Hello World", 325, 10, 20, BLACK);

    //TODO: impliment

    for (int i = 0; i < guardCount; i++)
    {
        Guard& theGuard = guards[i];

        Vector2 triVert0Copy = triVert0;
        Vector2 triVert1Copy = triVert1;
        Vector2 triVert2Copy = triVert2;

        //rotation
        float radians =  Game::radians(theGuard.getLerpRotation());

        float newVert0X = triVert0Copy.x * std::cosf(radians) - triVert0Copy.y * std::sinf(radians);
        float newVert0Y = triVert0Copy.x * std::sinf(radians) + triVert0Copy.y * std::cosf(radians);

        float newVert1X = triVert1Copy.x * std::cosf(radians) - triVert1Copy.y * std::sinf(radians);
        float newVert1Y = triVert1Copy.x * std::sinf(radians) + triVert1Copy.y * std::cosf(radians);

        float newVert2X = triVert2Copy.x * std::cosf(radians) - triVert2Copy.y * std::sinf(radians);
        float newVert2Y = triVert2Copy.x * std::sinf(radians) + triVert2Copy.y * std::cosf(radians);
        
        triVert0Copy.x = newVert0X;
        triVert0Copy.y = newVert0Y;

        triVert1Copy.x = newVert1X;
        triVert1Copy.y = newVert1Y;

        triVert2Copy.x = newVert2X;
        triVert2Copy.y = newVert2Y;

        //translation
        triVert0Copy.x += theGuard.getLerpPosX();
        triVert0Copy.y += theGuard.getLerpPosY();

        triVert1Copy.x += theGuard.getLerpPosX();
        triVert1Copy.y += theGuard.getLerpPosY();

        triVert2Copy.x += theGuard.getLerpPosX();
        triVert2Copy.y += theGuard.getLerpPosY();

        DrawTriangle(triVert0Copy, triVert1Copy, triVert2Copy, RED);
    }

    //do same for player

    Vector2 triVert0Copy = triVert0;
    Vector2 triVert1Copy = triVert1;
    Vector2 triVert2Copy = triVert2;

    //rotation
    float radians = Game::radians(thePlayer->getLerpRotation());

    float newVert0X = triVert0Copy.x * std::cosf(radians) - triVert0Copy.y * std::sinf(radians);
    float newVert0Y = triVert0Copy.x * std::sinf(radians) + triVert0Copy.y * std::cosf(radians);

    float newVert1X = triVert1Copy.x * std::cosf(radians) - triVert1Copy.y * std::sinf(radians);
    float newVert1Y = triVert1Copy.x * std::sinf(radians) + triVert1Copy.y * std::cosf(radians);

    float newVert2X = triVert2Copy.x * std::cosf(radians) - triVert2Copy.y * std::sinf(radians);
    float newVert2Y = triVert2Copy.x * std::sinf(radians) + triVert2Copy.y * std::cosf(radians);

    triVert0Copy.x = newVert0X;
    triVert0Copy.y = newVert0Y;

    triVert1Copy.x = newVert1X;
    triVert1Copy.y = newVert1Y;

    triVert2Copy.x = newVert2X;
    triVert2Copy.y = newVert2Y;

    //translation
    triVert0Copy.x += thePlayer->getLerpPosX();
    triVert0Copy.y += thePlayer->getLerpPosY();

    triVert1Copy.x += thePlayer->getLerpPosX();
    triVert1Copy.y += thePlayer->getLerpPosY();

    triVert2Copy.x += thePlayer->getLerpPosX();
    triVert2Copy.y += thePlayer->getLerpPosY();

    DrawTriangle(triVert0Copy, triVert1Copy, triVert2Copy, DARKBLUE);

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

float Game::radians(float degrees)
{
    return degrees * ((float)(std::_Pi) / 180.0F);
}
