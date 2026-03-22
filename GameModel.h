#pragma once
#ifndef GAME_MODEL_H
#define GAME_MODEL_H
#include "cocos2d.h"
#include "PileModel.h"
#include "GlobalEnum.h"
USING_NS_CC;

// M003：GameState数据模型 - 封装游戏核心状态（分数、剩余牌数、游戏进度）
class GameModel
{
public:
    GameModel();
    // M003 - 游戏状态核心字段
    int m_currentScore;    // 当前分数
    int m_mainPileRemain;  // 主牌堆剩余牌数
    int m_backupPileRemain;// 备用牌堆剩余牌数
    GameStatus m_status;   // 游戏进度（GAME_PLAYING/GAME_WIN/GAME_LOSE）
    // 牌堆模型
    PileModel m_mainPile;
    PileModel m_bottomPile;
    PileModel m_backupPile;
    // 常量
    const int kEliminateScore = 10; // 消除得分

    // 分数操作
    void addScore(int add = 10);
    void updatePileRemain();
    void setGameStatus(GameStatus status);
    inline bool isPlaying() const { return m_status == GAME_PLAYING; }
    inline bool isWin() const { return m_status == GAME_WIN; }
    inline bool isLose() const { return m_status == GAME_LOSE; }
    // 序列化/反序列化
    ValueMap serialize() const;
    void deserialize(const ValueMap& json);
};
#endif // GAME_MODEL_H