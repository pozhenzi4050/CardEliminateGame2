#pragma once
#include "Models/HistoryModel.h"
#include "GlobalEnum.h"

class HistoryService
{
public:
    static HistoryService* getInstance();
    static void destroyInstance();

    void setHistoryModel(HistoryModel* historyModel);

    bool undoLastOperation(GameModel* gameModel);

    // 检查是否有可回退的记录
    bool hasUndoRecord();
    void resetUndoTimes();
    // 获取剩余回退次数
    int getRemainUndoTimes();

private:
    HistoryService();
    ~HistoryService();
    static HistoryService* s_instance;

    void dispatchUndoEvent(const ValueMap& lastState);

    HistoryModel* m_historyModel = nullptr;
    int m_remainUndoTimes; // 剩余回退次数
};