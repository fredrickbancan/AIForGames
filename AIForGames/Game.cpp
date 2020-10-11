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
#include "math.h"
#include "PlayerController.h"


//triangle vertices
const Vector2 triVert0{ 0.0F, -15.F };
const Vector2 triVert1{ -10.0F, 10.F };
const Vector2 triVert2{ 10.0F, 10.F };

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
    playerController = PlayerController::get();
    guards = new Guard[guardCount];

    for (int i = 0; i < guardCount; i++)
    {
        guards[i].setPos(screenWidth  - (rand() % screenWidth), rand() %  screenHeight);
        guards[i].setRotation(rand() % 360);
    }
    buildLevelWalls();
    InitWindow(screenWidth, screenHeight, "Fredrick - AI for games");
}

Game::~Game()
{
    PlayerController::close();
    delete thePlayer;
    delete[] guards;
    delete ticksAndFps;
}

void Game::onFrame()
{
    if (closing = WindowShouldClose()) return;
    ticksAndFps->doOnTickUntillRealtimeSync(this);
    playerController->update();
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
        Vector2 guardLerpPos = theGuard.getLerpPos();
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
        triVert0Copy = Vector2Add(triVert0Copy, guardLerpPos);
        triVert1Copy = Vector2Add(triVert1Copy, guardLerpPos);
        triVert2Copy = Vector2Add(triVert2Copy, guardLerpPos);

        DrawTriangle(triVert0Copy, triVert1Copy, triVert2Copy, RED);
        //DEBUG front vector line
        DrawLine(guardLerpPos.x, guardLerpPos.y, guardLerpPos.x + theGuard.getLerpFrontVec().x * 50, guardLerpPos.y + theGuard.getLerpFrontVec().y * 50, DARKGREEN);
        //DEBUG aabb box
        drawAABB(theGuard.getAABB(), false);
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
    Vector2 playerLerpPos = thePlayer->getLerpPos();
    triVert0Copy = Vector2Add(triVert0Copy, playerLerpPos);
    triVert1Copy = Vector2Add(triVert1Copy, playerLerpPos);
    triVert2Copy = Vector2Add(triVert2Copy, playerLerpPos);

    DrawTriangle(triVert0Copy, triVert1Copy, triVert2Copy, DARKBLUE);
    //DEBUG front vector line
    DrawLine(playerLerpPos.x, playerLerpPos.y, playerLerpPos.x + thePlayer->getLerpFrontVec().x * 50, playerLerpPos.y + thePlayer->getLerpFrontVec().y * 50, DARKGREEN);
    //DEBUG aabb box
    drawAABB(thePlayer->getAABB(), false);

    //drawing all walls
    for (std::vector<AABB>::iterator i = levelWallBoxes.begin(); i != levelWallBoxes.end(); i++)
    {
        drawAABB(*i, true);
    }

    EndDrawing();
}

void Game::drawAABB(AABB box, bool wall)
{
    if (wall)
    {
        DrawRectangle(box.minBounds.x, box.minBounds.y, box.maxBounds.x - box.minBounds.x, box.maxBounds.y - box.minBounds.y, BLACK);
    }
    else
    {
        DrawRectangleLines(box.minBounds.x, box.minBounds.y, box.maxBounds.x - box.minBounds.x, box.maxBounds.y - box.minBounds.y, MAGENTA);
    }
}

void Game::tryToMoveEntity(EntityLiving* ent, Vector2& vel)
{
    //TODO: Impliment collisions
    Vector2 nextPos = Vector2Add(ent->getPos(), vel);//predicted position of entity
    for (std::vector<AABB>::iterator i = levelWallBoxes.begin(); i != levelWallBoxes.end(); i++)
    {
        //test if nextpos will collide with a box, if so, adjust velocity accordingly
    }

    ent->setPos(nextPos);//temp
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

void Game::buildLevelWalls()
{
    addWall(-10, 0, 10, screenHeight);//left screen bounds wall
    addWall(screenWidth - 10, 0, screenWidth + 10, screenHeight);//right screen bounds wall
    addWall(10, screenHeight - 10, screenWidth - 10, screenHeight + 10);//bottom screen bounds wall
    addWall(10, -10, screenWidth - 10, 10);//top screen bounds wall
}

void Game::addWall(float minX, float minY, float maxX, float maxY)
{
    levelWallBoxes.push_back(AABB(minX, minY, maxX, maxY));
}

float Game::radians(float degrees)
{
    return degrees * ((float)(3.141592653589F) / 180.0F);
}
