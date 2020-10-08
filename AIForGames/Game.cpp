#include "Game.h"

Game* Game::gameInstance = nullptr;

Game* Game::get()
{
    if (gameInstance == nullptr)
    {
        gameInstance = new Game();
    }
    return gameInstance;
}

Game::Game()
{
    //constructor
}

Game::~Game()
{
    if(gameInstance != nullptr)
    delete gameInstance;
}
