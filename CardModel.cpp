#include "CardModel.h"

CardModel::CardModel()
    : m_id(-1)
    , m_face(CFT_NONE)
    , m_suit(CST_NONE)
    , m_position(Vec2::ZERO)
    , m_isFront(false)
    , m_belongPile(PILE_NONE)
    , m_isEliminated(false)
    , m_pileIndex(0) // 初始化堆号
{
}

CardModel::CardModel(int id, CardFaceType face, CardSuitType suit)
    : m_id(id)
    , m_face(face)
    , m_suit(suit)
    , m_position(Vec2::ZERO)
    , m_isFront(false)
    , m_belongPile(PILE_NONE)
    , m_isEliminated(false)
    , m_pileIndex(0) // 初始化堆号
{
}

// 序列化：保存单张卡牌
ValueMap CardModel::serialize() const
{
    ValueMap json;
    json["id"] = Value(m_id);
    json["face"] = Value(static_cast<int>(m_face));
    json["suit"] = Value(static_cast<int>(m_suit));
    json["posX"] = Value(m_position.x);
    json["posY"] = Value(m_position.y);
    json["isFront"] = Value(m_isFront);
    json["belongPile"] = Value(static_cast<int>(m_belongPile));
    json["isEliminated"] = Value(m_isEliminated);
    json["pileIndex"] = Value(m_pileIndex); 
    return json;
}

// 反序列化：恢复单张卡牌
void CardModel::deserialize(const ValueMap& json)
{
    m_id = json.at("id").asInt();
    m_face = static_cast<CardFaceType>(json.at("face").asInt());
    m_suit = static_cast<CardSuitType>(json.at("suit").asInt());
    m_position.x = json.at("posX").asFloat();
    m_position.y = json.at("posY").asFloat();
    m_isFront = json.at("isFront").asBool();
    m_belongPile = static_cast<PileType>(json.at("belongPile").asInt());
    m_isEliminated = json.at("isEliminated").asBool();

    // 【关键】恢复堆号（兼容旧数据，如果没有这个key就给0）
    if (json.find("pileIndex") != json.end()) {
        m_pileIndex = json.at("pileIndex").asInt();
    }
    else {
        m_pileIndex = 0;
    }
}