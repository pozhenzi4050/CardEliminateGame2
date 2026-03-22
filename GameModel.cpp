#include "GameModel.h"
GameModel::GameModel()
    : m_currentScore(0), m_mainPileRemain(0), m_backupPileRemain(0), m_status(GAME_PLAYING)
    , m_mainPile(PILE_MAIN), m_bottomPile(PILE_BOTTOM), m_backupPile(PILE_BACKUP) {
}
void GameModel::addScore(int add) {
    m_currentScore += add;
}
void GameModel::updatePileRemain()
{
    m_mainPileRemain = m_mainPile.m_cardCount;
    m_backupPileRemain = m_backupPile.m_cardCount;
}
void GameModel::setGameStatus(GameStatus status) {
    m_status = status;
    // 同步更新牌堆剩余数（防止状态切换时数据不一致）
    updatePileRemain();
}
ValueMap GameModel::serialize() const
{
    ValueMap json;
    json["currentScore"] = m_currentScore;
    json["mainPileRemain"] = m_mainPileRemain;
    json["backupPileRemain"] = m_backupPileRemain;
    json["status"] = static_cast<int>(m_status);
    json["mainPile"] = m_mainPile.serialize();
    json["bottomPile"] = m_bottomPile.serialize();
    json["backupPile"] = m_backupPile.serialize();
    return json;
}
void GameModel::deserialize(const ValueMap& json)
{
    m_currentScore = json.at("currentScore").asInt();
    m_mainPileRemain = json.at("mainPileRemain").asInt();
    m_backupPileRemain = json.at("backupPileRemain").asInt();
    m_status = static_cast<GameStatus>(json.at("status").asInt());
    m_mainPile.deserialize(json.at("mainPile").asValueMap());
    m_bottomPile.deserialize(json.at("bottomPile").asValueMap());
    m_backupPile.deserialize(json.at("backupPile").asValueMap());
}