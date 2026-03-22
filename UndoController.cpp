#include "UndoController.h"


UndoController* UndoController::create()
{
    UndoController* controller = new UndoController();
    if (controller && controller->init())
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool UndoController::init()
{
    if (!Node::init()) return false;

    m_historyManager = HistoryManager::getInstance();

    registerUndoEvent();
    return true;
}



// 处理回退按钮点击
void UndoController::handleUndoOperation()
{
    CCLOG("[UndoController] ===== Undo button clicked =====");

    GameModel* gameModel = GameController::getInstance()->getGameModel();
    if (!gameModel) {
        CCLOG("[UndoController] Undo failed: GameModel is null");
        return;
    }

    // 调用 HistoryService 执行回退
    bool undoSuccess = HistoryService::getInstance()->undoLastOperation(gameModel);

    if (undoSuccess) {
        CCLOG("[UndoController] Undo success, remain times: %d", HistoryService::getInstance()->getRemainUndoTimes());
    }
    else {
        CCLOG("[UndoController] Undo failed");
    }
}

void UndoController::registerUndoEvent()
{
    // 监听游戏重启事件，重置回退系统
    GameEventDispatcher::getInstance()->addEventListener(EVENT_GAME_RESTART, this, [=](const GameEvent& event) {
        HistoryService::getInstance()->resetUndoTimes();
        HistoryManager::getInstance()->clearHistory();

        int remainTimes = HistoryService::getInstance()->getRemainUndoTimes();
        GameEventDispatcher::getInstance()->dispatchEvent({
            EVENT_UNDO_COUNT_CHANGED,
            {{"remainTimes", Value(remainTimes)}}
            });
        });
}
//void UndoController::handleUndoOperation()
//{
//    GameModel* gameModel = GameController::getInstance()->getGameModel();
//    if (!gameModel || !gameModel->isPlaying()) return;
//
//    // 1. 校验回退次数与历史记录
//    if (m_remainUndoTimes <= 0 || !m_historyManager->hasRecord())
//    {
//        return;
//    }
//
//    // 2. 委托HistoryManager获取上一步的游戏状态
//    ValueMap lastState = m_historyManager->undoLastRecord();
//    if (lastState.empty()) return;
//
//    // 3. 恢复游戏模型状态
//    gameModel->deserialize(lastState);
//
//    // 4. 扣减回退次数
//    m_remainUndoTimes--;
//
//    // 5. 分发事件，刷新所有视图与UI
//    GameEventDispatcher::getInstance()->dispatchEvent({
//        EVENT_UNDO_COMPLETED,
//        {}
//        });
//
//    GameEventDispatcher::getInstance()->dispatchEvent({
//        EVENT_PILE_UPDATED,
//        {}
//        });
//
//    GameEventDispatcher::getInstance()->dispatchEvent({
//        EVENT_UNDO_COUNT_CHANGED,
//        {{"remainTimes", Value(m_remainUndoTimes)}}
//        });
//}
