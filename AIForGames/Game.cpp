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
#include <string>
#include "math.h"
#include "PlayerController.h"
#include "NodeGraph.h"

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
    PlayerController::close();
    if (gameInstance != nullptr)
    {
        gameInstance->closeRaylib();
        delete gameInstance;
    }
}

Game::Game()
{
    ticksAndFps = new TicksAndFPS(30);
    ticksSinceGameEndNeededToRestart = ticksAndFps->getNumOfTicksForSeconds(3);
    thePlayer = new Player(30, screenHeight - 30, 0);
    playerController = PlayerController::get();
    guards = new Guard[guardCount];
    nodeGraph = new NodeGraph(screenWidth / 25, screenHeight / 25, 25);
    escapeTrigger = AABB(10, screenHeight-60, 100, screenHeight - 10);
    goldTrigger = AABB((screenWidth / 2) - 5, (screenHeight / 2) - 55, (screenWidth / 2) + 5, (screenHeight / 2) - 45);

    InitWindow(screenWidth, screenHeight, "Fredrick - AI for games");
    buildLevelWalls();
    loadGuardsAndResetGame();
    nodeGraph->linkNodes(&levelWallBoxes);
}

Game::~Game()
{
    delete nodeGraph;
    delete thePlayer;
    delete[] guards;
    delete ticksAndFps;
}

void Game::onFrame()
{
    toggleBooleanOnButtonPress(IsKeyPressed(KEY_F4), drawDebug);//toggle drawing debug if f4 pressed
    if (closing = WindowShouldClose()) return;
    ticksAndFps->doOnTickUntillRealtimeSync(this);
    playerController->update();
    ticksAndFps->updateFPS();
}

void Game::onTick()
{
    if (AABB::touching(goldTrigger, *thePlayer->getAABB()))
    {
        playerHasGold = true;
    }

    if (playerHasGold && AABB::touching(escapeTrigger, *thePlayer->getAABB()))
    {
        gameWon = true;
    }

    thePlayer->onTick();

    if (!(gameWon || gameLost))//only update the guards if the game is running
    {
        for (int i = 0; i < guardCount; i++)
        {
            guards[i].onTick();
            //test if guard is touching player, if so, game lost.
            if (AABB::touching(*guards[i].getAABB(), *thePlayer->getAABB()))
            {
                gameLost = true;
            }
        }
    }

    if (gameWon || gameLost)
    {
        ticksSinceGameEnd++;
        if (ticksSinceGameEnd >= ticksSinceGameEndNeededToRestart)
        {
            loadGuardsAndResetGame();//will reset game and set gamewon and gamelost to false
        }
    }
}

void Game::drawScene()
{
    BeginDrawing();
    
    ClearBackground(LIGHTGRAY);
    
    DrawText("Use arrow keys to move", screenWidth - 260, 10, 20, BLACK);
    DrawText("Press F4 to draw hitboxes", screenWidth - 290, 30, 20, BLACK);

    if(drawDebug)
    nodeGraph->debugDrawNodes();

    //game state based displays
    if (playerHasGold)//if the player has picked up the gold
    {
        DrawText("Escape", 20, screenHeight - 40, 20, ORANGE);//text at spawn / escape spot
        DrawText("Escape with the gold!", 300, 20, 20, ORANGE);//text at top of screen
    }
    else
    {
        DrawText("Start", 20, screenHeight - 40, 20, RED);//text at spawn / escape spot
        DrawText("Steal the gold!", 330, 20, 20, BLACK);//text at top of screen
        DrawRectangle((screenWidth/2)-5, (screenHeight/2)-55, 10,10,ORANGE);//gold in middle of level
    }
    
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
        if (drawDebug)
        {
            //DEBUG front vector line
            DrawLine(guardLerpPos.x, guardLerpPos.y, guardLerpPos.x + theGuard.getLerpFrontVec().x * 50, guardLerpPos.y + theGuard.getLerpFrontVec().y * 50, DARKGREEN);
            //DEBUG aabb box
            drawAABB(*theGuard.getAABB(), false);
        }
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
    if (drawDebug)
    {
        //DEBUG front vector line
        DrawLine(playerLerpPos.x, playerLerpPos.y, playerLerpPos.x + thePlayer->getLerpFrontVec().x * 50, playerLerpPos.y + thePlayer->getLerpFrontVec().y * 50, DARKGREEN);
        //DEBUG aabb box
        drawAABB(*thePlayer->getAABB(), false);

        //DEBUG draw escape trigger
        drawAABB(escapeTrigger, false);

        //DEBUG draw gold trigger
        drawAABB(goldTrigger, false);
    }

    //drawing all walls
    for (std::vector<AABB>::iterator i = levelWallBoxes.begin(); i != levelWallBoxes.end(); i++)
    {
        drawAABB(*i, true);
    }

    if (gameWon)
    {
        DrawRectangle(20 , 20, screenWidth - 40, screenHeight - 40, ORANGE);//Rectangle to cover screen
        DrawText("You Won!", (screenWidth/2) - 200, (screenHeight/2) - 60, 100, BLACK);//YOU WON text
    }
    else if (gameLost)
    {
        DrawRectangle(20, 20, screenWidth - 40, screenHeight - 40, RED);//Rectangle to cover screen
        DrawText("You Lost!", (screenWidth / 2) - 220, (screenHeight / 2) - 60, 100, BLACK);//YOU WON text
    }

    DrawText((std::string("FPS: ") + std::to_string(ticksAndFps->getFps())).c_str(), 15, 10, 20, DARKGREEN);
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

void Game::tryToMoveEntity(EntityLiving* ent, Vector2& entVel)
{
    AABB* entBox = ent->getAABB();
    
	/*test if entity bounding box aligns with a wall, if so, correct velocity so
	  it will not go through the wall in the way.*/

	//correct y velocity
    for (std::vector<AABB>::iterator wall = levelWallBoxes.begin(); wall != levelWallBoxes.end(); wall++)
    {
        if (AABB::overlappingX(*entBox, *wall))
        {
            float yDist;//distance between boxes in y direction, depending on position and velocity

            //if ent is moving towards positive y and entity box is "below" wall box
			if (entVel.y > 0.0F && entBox->maxBounds.y <= wall->minBounds.y)
			{
                yDist = wall->minBounds.y - entBox->maxBounds.y;
                if (yDist < entVel.y)
                {
                    entVel.y = yDist;
                }
			}

            //if ent is moving towards negative y and entity box is "above" wall box
            if (entVel.y < 0.0F && entBox->minBounds.y >= wall->maxBounds.y)
            {
                yDist = wall->maxBounds.y - entBox->minBounds.y;//creating negative dist for comparing with negative velocity so there is no need to use abs() func
                if (yDist > entVel.y)
                {
                    entVel.y = yDist;
                }
            }
        }

    }

    entBox->offset(Vector2{0, entVel.y});//move bounding box accordingly with new y velocity

    //correct x velocity
    for (std::vector<AABB>::iterator wall = levelWallBoxes.begin(); wall != levelWallBoxes.end(); wall++)
    {
        if (AABB::overlappingY(*entBox, *wall))
        {
            float xDist;//distance between boxes in x direction, depending on position and velocity

             //if ent is moving towards positive x and entity box is "to left" of wall box
            if (entVel.x > 0.0F && entBox->maxBounds.x <= wall->minBounds.x)
            {
                xDist = wall->minBounds.x - entBox->maxBounds.x;
                if (xDist < entVel.x)
                {
                    entVel.x = xDist;
                }
            }

            //if ent is moving towards negative x and entity box is "to right" of wall box
            if (entVel.x < 0.0F && entBox->minBounds.x >= wall->maxBounds.x)
            {
                xDist = wall->maxBounds.x - entBox->minBounds.x;//creating negative dist for comparing with negative velocity so there is no need to use abs() func
                if (xDist > entVel.x)
                {
                    entVel.x = xDist;
                }
            }
        }
    }

    entBox->offset(Vector2{ entVel.x, 0 });//move bounding box accordingly with new x velocity     
    
    ent->setPos(entBox->getCenter());//set entity position to align with its new bounding box position
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

    addWall(100, screenHeight - 70, 110, screenHeight - 10);//spawn room right wall
    addWall(10, screenHeight - 70, 55, screenHeight - 60);//spawn room upper wall

    addWall(100, screenHeight - 200, screenWidth - 100, screenHeight - 190);//main room bottom wall
    addWall(screenWidth - 110, 100, screenWidth - 100, screenHeight - 200);//main room right wall
    addWall(100, 100, 110, screenHeight - 200);//main room left wall
    addWall(110, 100, 350, 110);//main upper left wall
    addWall(screenWidth - 350, 100, screenWidth - 110, 110);//main upper right wall

    addWall(200, 160, screenWidth - 200, 170);//inner room upper wall
    addWall(200, 170, 210, screenHeight - 260);//inner room left wall
    addWall(screenWidth - 210, 170, screenWidth - 200, screenHeight - 260);//inner room right wall
    addWall(200, screenHeight - 260, screenWidth - 450, screenHeight - 250);//inner room bottom left wall
    addWall(screenWidth - 350, screenHeight - 260, screenWidth - 200, screenHeight - 250);//inner room bottom right wall

    addWall(screenWidth - 450, screenHeight - 60, screenWidth - 400, screenHeight - 10);//bottom middle crate
    addWall(screenWidth - 250, screenHeight - 120, screenWidth - 200, screenHeight - 70);//bottom middle right crate
    addWall(screenWidth - 50, screenHeight - 240, screenWidth - 10, screenHeight - 200);//right lower crate
    addWall(screenWidth - 100, screenHeight - 440, screenWidth - 60, screenHeight - 400);//right upper crate
    addWall(60, screenHeight - 240, 100, screenHeight - 200);//left lower crate
    addWall(10, screenHeight - 440, 50, screenHeight - 400);//left upper crate
    addWall(300, 230, 350, 280);//inner middle left crate
    addWall(screenWidth - 350, 230, screenWidth - 300, 280);//inner middle right crate
    addWall(350, 270, screenWidth - 350, 280);//inner middle between-crate wall
}

void Game::addWall(float minX, float minY, float maxX, float maxY)
{
    levelWallBoxes.push_back(AABB(minX, minY, maxX, maxY));
}

void Game::loadGuardsAndResetGame()
{
    playerHasGold = false;
    gameWon = false;
    gameLost = false;
    ticksSinceGameEnd = 0;
    for (int i = 0; i < guardCount; i++)
    {
        guards[i] = Guard((float)(screenWidth - 20 - (rand() % (screenWidth - 20))), (float)(rand() % (screenHeight - 40)) + 20.0F, (float)(rand() % 360));
    }
    thePlayer->setPos(30, screenHeight - 30);
    thePlayer->setRotation(0);
}

float Game::radians(float degrees)
{
    return degrees * ((float)(3.141592653589F) / 180.0F);
}
