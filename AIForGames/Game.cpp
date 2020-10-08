#include "Game.h"
Game* Game::gameInstance = nullptr;
#include "TicksAndFPS.h"

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
        delete gameInstance;
    }
}

Game::Game()
{
    ticksAndFps = new TicksAndFPS(30);
}

Game::~Game()
{
    delete ticksAndFps;
}

void Game::onFrame()
{
    ticksAndFps->doOnTickUntillRealtimeSync(onTick);
}

void onTick()
{
    //TODO: impliment
}

void Game::drawScene()
{
    //TODO: impliment
}