#include "HistoryManager.h"

HistoryManager* HistoryManager::s_instance = nullptr;

HistoryManager* HistoryManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new HistoryManager();
    }
    return s_instance;
}

void HistoryManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

void HistoryManager::addRecord(OperateType type, const GameModel& gameModel)
{
    m_historyModel.addRecord(type, gameModel);
}

ValueMap HistoryManager::undoLastRecord()
{
    return m_historyModel.undoLastRecord();
}

void HistoryManager::clearHistory()
{
    m_historyModel.clearHistory();
}