#pragma once
#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "cocos2d.h"
#include "../Models/GameModel.h"
#include "EventDispatcher.h"
USING_NS_CC;

// 前置声明，避免循环引用
class PileManager;
class GameStateManager;
class PileView;
class UIFeedbackView;
class UndoBtnView;

class GameController : public Node
{
public:
    static GameController* create();
    static GameController* getInstance();
    bool init() override;

    // 核心生命周期方法
    void initGame(); // 初始化游戏
    void restartGame(); // 重启游戏
    void gameOver(bool isWin); // 游戏结束处理

    // 公有获取方法
    inline GameModel* getGameModel() { return m_gameModel; }
    void onMainCardClicked(const CardModel* clickCard);
    void onBackupPileClicked();

private:
    GameController();
    ~GameController();
    static GameController* s_instance;

    // 核心成员
    GameModel* m_gameModel;
    // 子控制器
    class CardClickController* m_cardClickController;
    class UndoController* m_undoController;
    // Manager层引用
    PileManager* m_pileManager;
    GameStateManager* m_gameStateManager;
    // View层引用
    PileView* m_mainPileView;
    PileView* m_bottomPileView;
    PileView* m_backupPileView;
    UIFeedbackView* m_uiFeedbackView;
    UndoBtnView* m_undoBtnView;

    // 私有方法
    void initManagers(); // 初始化管理器
    void initViews(); // 初始化视图
    void initControllers(); // 初始化子控制器
    void registerGlobalEvents(); // 注册全局事件监听
    void unregisterGlobalEvents(); // 注销全局事件监听
};

#endif // GAME_CONTROLLER_H