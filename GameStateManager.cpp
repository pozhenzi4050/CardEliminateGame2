#include "GameStateManager.h"
#include "MatchManager.h" 

GameStateManager* GameStateManager::s_instance = nullptr;

GameStateManager* GameStateManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new GameStateManager();
    }
    return s_instance;
}

void GameStateManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

GameStateManager::GameStateManager()
{
    m_matchManager = MatchManager::getInstance();
}

void GameStateManager::resetGameState(GameModel* gameModel)
{

}

bool GameStateManager::hasMatchableCard(GameModel* gameModel)
{

}

void GameStateManager::checkGameEnd(GameModel* gameModel)
{
   
}