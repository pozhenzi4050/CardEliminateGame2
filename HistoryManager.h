#pragma once
#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H
#include "cocos2d.h"
#include "../Models/HistoryModel.h"
#include "../Models/GameModel.h"
#include "GlobalEnum.h"
USING_NS_CC;

class HistoryManager
{
public:
    static HistoryManager* getInstance();
    static void destroyInstance();

    void addRecord(OperateType type, const GameModel& gameModel);

    ValueMap undoLastRecord();

    void clearHistory();

    // ╠Ц╫щеп╤о
    inline bool hasRecord() const { return m_historyModel.hasRecord(); }

private:
    HistoryManager() = default;
    ~HistoryManager() = default;
    static HistoryManager* s_instance;

    HistoryModel m_historyModel; 
};

#endif // HISTORY_MANAGER_H