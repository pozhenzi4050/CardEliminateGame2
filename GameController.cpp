#include "GameController.h"
#include "CardClickController.h"
#include "UndoController.h"
#include "../Managers/PileManager.h"
#include "../Managers/GameStateManager.h"
#include "../Views/PileView.h"
#include "../Views/UIFeedbackView.h"
#include "../Views/UndoBtnView.h"
#include "GlobalEnum.h"
#include "cocos2d.h"
GameController* GameController::s_instance = nullptr;

GameController* GameController::getInstance() {
    if (!s_instance) {
        // 先创建实例
        s_instance = new GameController();
        // 手动调用init（create()会自动调用，但new不会）
        if (!s_instance->init()) {
            delete s_instance;
            s_instance = nullptr;
            CCLOGERROR("[GameController] 单例初始化失败！");
        }
    }
    int remainTimes = HistoryService::getInstance()->getRemainUndoTimes();
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_UNDO_COUNT_CHANGED,
        {{"remainTimes", Value(remainTimes)}}
        });
    return s_instance;
}

GameController* GameController::create()
{
    GameController* controller = new GameController();
    if (controller && controller->init())
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

GameController::GameController()
{
    s_instance = this;
    m_gameModel = new GameModel();
    m_cardClickController = nullptr;
    m_undoController = nullptr;
    m_pileManager = nullptr;
    m_gameStateManager = nullptr;
    m_mainPileView = nullptr;
    m_bottomPileView = nullptr;
    m_backupPileView = nullptr;
    m_uiFeedbackView = nullptr;
    m_undoBtnView = nullptr;
}

GameController::~GameController()
{
    unregisterGlobalEvents();
    GameEventDispatcher::destroyInstance();
    CC_SAFE_DELETE(m_gameModel);
    s_instance = nullptr;
}

bool GameController::init()
{
    if (!Node::init()) return false;

    initManagers();
    initGame();
    initControllers();
    initViews();
    registerGlobalEvents();
    m_undoBtnView->resetUndoCount();
    return true;

}
// GameController.cpp 新增实现
void GameController::onMainCardClicked(const CardModel* clickCard) {
    if (!clickCard || !m_cardClickController) return;
    m_cardClickController->handleMainCardClick(clickCard);
}

void GameController::onBackupPileClicked() {
    if (!m_cardClickController) return;
    m_cardClickController->handleBackupPileClick();
}

void GameController::initManagers()
{
    m_pileManager = PileManager::getInstance();
    m_gameStateManager = GameStateManager::getInstance();
}

void GameController::initControllers()
{
    // 初始化子控制器
    m_cardClickController = CardClickController::create();
    this->addChild(m_cardClickController);

    m_undoController = UndoController::create();
    this->addChild(m_undoController);
}

void GameController::initViews()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 1. 初始化UI反馈视图（全局层级最高）
    m_uiFeedbackView = UIFeedbackView::create();
    this->addChild(m_uiFeedbackView, 100);

    // 2. 初始化主牌堆视图
    m_mainPileView = PileView::create(&m_gameModel->m_mainPile);
    m_mainPileView->setPosition(Vec2(0, visibleSize.height - 1000));
    m_mainPileView->onCardClick = [=](const CardModel* card, PileType pileType) {
        onMainCardClicked(card);
        };
    this->addChild(m_mainPileView);

    // 3. 初始化底牌堆视图
    m_bottomPileView = PileView::create(&m_gameModel->m_bottomPile);
    m_bottomPileView->setPosition(Vec2(640, 100));
    this->addChild(m_bottomPileView);

    // 4. 初始化备用牌堆视图
    m_backupPileView = PileView::create(&m_gameModel->m_backupPile);
    m_backupPileView->setPosition(Vec2(240, 100));
    m_backupPileView->onPileClick = [=](PileType pileType) {
        onBackupPileClicked();
        };
    this->addChild(m_backupPileView);

    // 5. 初始化回退按钮视图
    m_undoBtnView = UndoBtnView::create();
    m_undoBtnView->setPosition(Vec2(840, 100));
    m_undoBtnView->onUndoBtnClick = [=]() {
        m_undoController->handleUndoOperation();
        };
    this->addChild(m_undoBtnView);
}

void GameController::registerGlobalEvents()
{
    auto dispatcher = GameEventDispatcher::getInstance();

    // 刷新所有视图
    dispatcher->addEventListener(EVENT_PILE_UPDATED, this, [=](const GameEvent& event) {
        CCLOG("[GameController] ===== Received view refresh request (after undo/operation) =====");

        // 检查 PileView 是否已初始化
        if (!m_mainPileView || !m_bottomPileView || !m_backupPileView) {
            CCLOGERROR("[GameController] Refresh failed: PileView not initialized!");
            return;
        }

        // 检查 GameModel 是否有效
        if (!m_gameModel) {
            CCLOGERROR("[GameController] Refresh failed: GameModel is null!");
            return;
        }

        // 重新绑定 Model，防止野指针
        m_mainPileView->setPileModel(&m_gameModel->m_mainPile);
        m_bottomPileView->setPileModel(&m_gameModel->m_bottomPile);
        m_backupPileView->setPileModel(&m_gameModel->m_backupPile);

        // 强制清空并重建所有卡牌
        m_mainPileView->clearAllCards();
        m_bottomPileView->clearAllCards();
        m_backupPileView->clearAllCards();

        m_mainPileView->createCards();
        m_bottomPileView->createCards();
        m_backupPileView->createCards();

        CCLOG("[GameController] View refresh completed: MainPile=%d, BottomPile=%d, BackupPile=%d",
            (int)m_gameModel->m_mainPile.m_cards.size(),
            (int)m_gameModel->m_bottomPile.m_cards.size(),
            (int)m_gameModel->m_backupPile.m_cards.size());
        });

    // 监听分数变更事件
    dispatcher->addEventListener(EVENT_SCORE_CHANGED, this, [=](const GameEvent& event) {
        int addScore = event.userData.at("addScore").asInt();
        Vec2 worldPos = Vec2(
            event.userData.at("posX").asFloat(),
            event.userData.at("posY").asFloat()
        );
        m_uiFeedbackView->playScoreTip(worldPos, StringUtils::format("+%d", addScore));
        });

    // 监听游戏状态变更事件
    dispatcher->addEventListener(EVENT_GAME_STATE_CHANGED, this, [=](const GameEvent& event) {
        GameStatus status = static_cast<GameStatus>(event.userData.at("status").asInt());
        if (status == GAME_WIN)
        {
            gameOver(true);
        }
        else if (status == GAME_LOSE)
        {
            gameOver(false);
        }
        });

    // 监听回退次数变更事件
    dispatcher->addEventListener(EVENT_UNDO_COUNT_CHANGED, this, [=](const GameEvent& event) {
        int remainTimes = event.userData.at("remainTimes").asInt();
        m_undoBtnView->updateUndoCountShow(remainTimes);
        });
}

void GameController::initGame()
{
    CCLOG("[GameController] ===== Start init game =====");

    // 1. 重置游戏模型
    CC_SAFE_DELETE(m_gameModel);
    m_gameModel = new GameModel();

    // 2. 调用 Manager 初始化牌堆
    m_pileManager->initPiles(m_gameModel);

    // 3. 重置游戏状态
    m_gameStateManager->resetGameState(m_gameModel);

    // ==========================================
    // 【重置回退系统】
    // ==========================================
    HistoryService::getInstance()->resetUndoTimes();
    HistoryManager::getInstance()->clearHistory();

    if (m_undoBtnView) {
        m_undoBtnView->resetUndoCount();
    }

    int remainTimes = HistoryService::getInstance()->getRemainUndoTimes();
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_UNDO_COUNT_CHANGED,
        {{"remainTimes", Value(remainTimes)}}
        });
    // ==========================================

    // 4. 给 PileView 重新绑定新的牌堆模型
    if (m_mainPileView) {
        m_mainPileView->setPileModel(&m_gameModel->m_mainPile);
        CCLOG("[GameController] Main pile rebind completed, count=%d", (int)m_gameModel->m_mainPile.m_cards.size());
    }
    if (m_bottomPileView) {
        m_bottomPileView->setPileModel(&m_gameModel->m_bottomPile);
    }
    if (m_backupPileView) {
        m_backupPileView->setPileModel(&m_gameModel->m_backupPile);
    }

    // 5. 刷新所有视图
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_PILE_UPDATED,
        {}
        });

    CCLOG("[GameController] ===== Game init completed =====");
}
void GameController::unregisterGlobalEvents()
{
    GameEventDispatcher::getInstance()->removeAllListenersForTarget(this);
}


void GameController::restartGame()
{
    initGame();
    m_undoBtnView->resetUndoCount(); // 重置回退按钮视图
    m_cardClickController->setEnabled(true); // 恢复交互
}

void GameController::gameOver(bool isWin)
{
    // 暂停游戏
    m_gameModel->setGameStatus(isWin ? GAME_WIN : GAME_LOSE);
    // 禁用交互
    m_cardClickController->setEnabled(false);
    m_undoBtnView->setEnabled(false);
    // 弹出胜负弹窗
    m_uiFeedbackView->showGameResultPopup(isWin, m_gameModel->m_currentScore, [=]() {
        restartGame();
        m_cardClickController->setEnabled(true);
        });
}