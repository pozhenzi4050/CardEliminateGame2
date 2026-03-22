#pragma once
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H
#include "cocos2d.h"
#include "../Models/GameModel.h"
#include "GlobalEnum.h"
USING_NS_CC;

// 前置声明
class MatchManager;

class GameStateManager
{
public:
    static GameStateManager* getInstance();
    static void destroyInstance();

    // 重置游戏状态
    void resetGameState(GameModel* gameModel);

    // 检查游戏是否结束（胜利/失败）
    void checkGameEnd(GameModel* gameModel);

private:
    GameStateManager();
    ~GameStateManager() = default;
    static GameStateManager* s_instance;

    MatchManager* m_matchManager;

    // 检查主牌堆是否有可匹配的牌
    bool hasMatchableCard(GameModel* gameModel);
};

#endif // GAME_STATE_MANAGER_H