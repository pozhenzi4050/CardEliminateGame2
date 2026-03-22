#include "HistoryModel.h"

ValueMap HistoryRecord::serialize() const
{
    ValueMap json;
    json["operateType"] = static_cast<int>(operateType);
    json["gameState"] = gameState;
    return json;
}

void HistoryRecord::deserialize(const ValueMap& json)
{
    operateType = static_cast<OperateType>(json.at("operateType").asInt());
    gameState = json.at("gameState").asValueMap();
}


HistoryModel::HistoryModel()
{
    m_historyList.clear();
}

void HistoryModel::addRecord(OperateType type, const GameModel& gameModel)
{
    // 超过最大记录数，删除最旧的一条
    if (m_historyList.size() >= kMaxHistoryCount)
    {
        m_historyList.erase(m_historyList.begin());
    }
    // 构造历史记录
    HistoryRecord record;
    record.operateType = type;
    record.gameState = gameModel.serialize();
    m_historyList.push_back(record);
}

ValueMap HistoryModel::undoLastRecord()
{
    if (m_historyList.empty())
    {
        return ValueMap();
    }
    // 获取最后一条记录的游戏状态
    ValueMap lastState = m_historyList.back().gameState;
    // 删除最后一条记录
    m_historyList.pop_back();
    return lastState;
}

void HistoryModel::clearHistory()
{
    m_historyList.clear();
}

ValueMap HistoryModel::serialize() const
{
    ValueMap json;
    ValueVector historyVec;
    for (const auto& record : m_historyList)
    {
        historyVec.push_back(Value(record.serialize()));
    }
    json["historyCount"] = m_historyList.size();
    json["historyList"] = historyVec;
    return json;
}

void HistoryModel::deserialize(const ValueMap& json)
{
    m_historyList.clear();
    int count = json.at("historyCount").asInt();
    ValueVector historyVec = json.at("historyList").asValueVector();
    for (const auto& recordVal : historyVec)
    {
        HistoryRecord record;
        record.deserialize(recordVal.asValueMap());
        m_historyList.push_back(record);
    }
}
