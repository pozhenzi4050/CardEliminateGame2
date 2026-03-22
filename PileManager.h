#pragma once
#ifndef PILE_MANAGER_H
#define PILE_MANAGER_H
#include "cocos2d.h"
#include "../Models/GameModel.h"
#include "../Models/CardModel.h"
#include "GlobalEnum.h"
USING_NS_CC;

// 前置声明
class CardFlipManager;

class PileManager
{
public:
    static PileManager* getInstance();
    static void destroyInstance();
    // 新增：获取备用牌顶牌原始位置
    Vec2 getBackupCardOriginPos(GameModel* gameModel);

    // G001核心1：初始化所有牌堆（生成52张牌、洗牌、发牌）
    void initPiles(GameModel* gameModel);

    void eliminateCard(GameModel* gameModel, const CardModel* cardModel);

    bool flipBackupCard(GameModel* gameModel);
    void PileManager::shuffleCards(std::vector<CardModel>& cards);

private:
    PileManager();
    ~PileManager() = default;
    static PileManager* s_instance;

    CardFlipManager* m_cardFlipManager;

};

#endif // PILE_MANAGER_H