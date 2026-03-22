#include "HistoryService.h"
#include "Controllers/EventDispatcher.h"
#include "Managers/HistoryManager.h"
#include "Controllers/GameController.h"

HistoryService* HistoryService::s_instance = nullptr;

HistoryService::HistoryService()
{
    m_remainUndoTimes = MAX_UNDO_TIMES;
}

HistoryService::~HistoryService()
{
}

void HistoryService::resetUndoTimes()
{
    m_remainUndoTimes = MAX_UNDO_TIMES;
}

HistoryService* HistoryService::getInstance()
{
    if (!s_instance)
    {
        s_instance = new HistoryService();
    }
    return s_instance;
}

void HistoryService::destroyInstance()
{
    if (s_instance)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}
bool HistoryService::undoLastOperation(GameModel* gameModel)
{
    CCLOG("[HistoryService] ================ Start undo operation ================");

    HistoryManager* historyManager = HistoryManager::getInstance();

    // 【校验回退条件】
    if (!gameModel) {
        CCLOG("[HistoryService] Undo failed: GameModel is null");
        return false;
    }
    if (m_remainUndoTimes <= 0) {
        CCLOG("[HistoryService] Undo failed: no remain undo times");
        return false;
    }
    if (!historyManager->hasRecord()) {
        CCLOG("[HistoryService] Undo failed: no history record");
        return false;
    }

    // 1. 获取上一次的历史状态
    ValueMap lastState = historyManager->undoLastRecord();
    if (lastState.empty()) {
        CCLOG("[HistoryService] Undo failed: history state is empty");
        return false;
    }

    CCLOG("[HistoryService] Get history state success, start restore data...");

    // 2. 恢复 GameModel 数据
    gameModel->deserialize(lastState);

    CCLOG("[HistoryService] Data restore completed: MainPile=%d, BottomPile=%d, BackupPile=%d",
        (int)gameModel->m_mainPile.m_cards.size(),
        (int)gameModel->m_bottomPile.m_cards.size(),
        (int)gameModel->m_backupPile.m_cards.size());

    // 3. 扣减回退次数
    m_remainUndoTimes--;

    // 4. 分发事件（只分发这两个，避免混乱）
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_UNDO_COUNT_CHANGED,
        {{"remainTimes", Value(m_remainUndoTimes)}}
        });

    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_PILE_UPDATED,
        {}
        });

    CCLOG("[HistoryService] ================ Undo success (remain times: %d) ================", m_remainUndoTimes);
    return true;
}
 
void HistoryService::setHistoryModel(HistoryModel* historyModel)
{
    m_historyModel = historyModel;
}


// 检查是否有可回退记录
bool HistoryService::hasUndoRecord()
{
    return HistoryManager::getInstance()->hasRecord();
}

// 获取剩余回退次数
int HistoryService::getRemainUndoTimes()
{
    return m_remainUndoTimes;
}

// 内部工具：分发回退事件
void HistoryService::dispatchUndoEvent(const ValueMap& lastState)
{
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_UNDO_TRIGGERED,
        {{"lastState", Value(lastState)}}
        });
}