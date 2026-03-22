#pragma once
#include "cocos2d.h"
#include "Card.h"
#include "Pile.h"
#include "CardUtils.h"
USING_NS_CC;

// 游戏状态枚举
enum GameStatus {
    GAME_READY,    // 准备中
    GAME_PLAYING,  // 游戏中
    GAME_WIN,      // 胜利
    GAME_LOSE      // 失败
};

// 游戏管理器（单例模式，全局唯一）
class GameManager {
public:
    // 获取单例（全局唯一实例）
    static GameManager* getInstance();
    // 销毁单例
    static void destroyInstance();

    // 初始化游戏（生成牌堆、洗牌、布局）
    void initGame();

    // 核心游戏逻辑
    bool eliminateCard(Card* card); // 消除指定牌
    void drawCardFromStock();       // 从备用牌堆抽牌到底牌堆
    void checkGameStatus();         // 检查游戏胜负状态

    // Get方法
    Pile* getMainPile() { return m_mainPile; }
    Pile* getDeckPile() { return m_deckPile; }
    Pile* getStockPile() { return m_stockPile; }
    GameStatus getGameStatus() { return m_gameStatus; }
    int getScore() { return m_score; }

private:
    // 单例私有构造/析构
    GameManager();
    ~GameManager();
    static GameManager* s_instance; // 单例实例

    // 核心成员
    Pile* m_mainPile;    // 主牌堆
    Pile* m_deckPile;    // 底牌堆
    Pile* m_stockPile;   // 备用牌堆
    GameStatus m_gameStatus; // 游戏状态
    int m_score;         // 游戏分数
};