#include "GamePch.h"
#include "Game.h"

#include "GameTags.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/ComponentSystem/ComponentSystem.h"

Game::Game()
{
    check(myInstance == nullptr);
    myInstance = this;
}

Game::~Game()
{
    myInstance = nullptr;
}

void Game::Init()
{
}

void Game::StaticTick()
{
    myInstance->Tick();
}

void Game::Tick()
{
}
