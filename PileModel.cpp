#include "PileModel.h"

PileModel::PileModel()
    : m_type(PILE_NONE)
    , m_cardCount(0)
{
}

PileModel::PileModel(PileType type)
    : m_type(type)
    , m_cardCount(0)
{
}

void PileModel::addCard(const CardModel& card)
{
    m_cards.push_back(card);
    m_cardCount = static_cast<int>(m_cards.size());
}

void PileModel::removeCard(const CardModel* card)
{
    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        if (it->m_id == card->m_id) {
            m_cards.erase(it);
            break;
        }
    }
    m_cardCount = static_cast<int>(m_cards.size());
}

CardModel* PileModel::getTopCard()
{
    if (m_cards.empty()) return nullptr;
    return &m_cards.back();
}

void PileModel::clear()
{
    m_cards.clear();
    m_cardCount = 0;
}

ValueMap PileModel::serialize() const
{
    ValueMap json;
    json["type"] = Value(static_cast<int>(m_type));
    json["cardCount"] = Value(m_cardCount);

    ValueVector cardList;
    for (const auto& card : m_cards) {
        cardList.push_back(Value(card.serialize()));
    }
    json["cards"] = Value(cardList);

    return json;
}

void PileModel::deserialize(const ValueMap& json)
{
    m_type = static_cast<PileType>(json.at("type").asInt());
    m_cardCount = json.at("cardCount").asInt();
    m_cards.clear();

    // »Ö¸´ËùÓÐ¿¨ÅÆ
    if (json.find("cards") != json.end()) {
        ValueVector cardList = json.at("cards").asValueVector();
        for (const auto& cardVal : cardList) {
            CardModel card;
            card.deserialize(cardVal.asValueMap());
            m_cards.push_back(card);
        }
    }
}