#pragma once
#ifndef HISTORY_MODEL_H
#define HISTORY_MODEL_H
#include "cocos2d.h"
#include "GlobalEnum.h"
#include "GameModel.h"
#include"CardModel.h"
#include <vector>
USING_NS_CC;

class HistoryRecord
{
public:
    HistoryRecord() : operateType(OP_ELIMINATE) {}
    OperateType operateType; // 操作类型（消除/翻牌）
    ValueMap gameState;      // 操作前的完整游戏状态（GameModel序列化结果）
    // 序列化/反序列化
    ValueMap serialize() const;
    void deserialize(const ValueMap& json);
};

class HistoryModel
{
public:
    HistoryModel();
    // 与回退按钮最大次数保持一致，避免逻辑冲突
    const int kMaxHistoryCount = 5;
    std::vector<HistoryRecord> m_historyList;

    void addRecord(OperateType type, const GameModel& gameModel);
    ValueMap undoLastRecord();
    // 清空历史（游戏重启/重开时调用）
    void clearHistory();
    // 便捷判断
    inline bool hasRecord() const { return !m_historyList.empty(); }
    inline int getRecordCount() const { return m_historyList.size(); }
    // 序列化/反序列化（存档用）
    ValueMap serialize() const;
    void deserialize(const ValueMap& json);
};
#endif // HISTORY_MODEL_H