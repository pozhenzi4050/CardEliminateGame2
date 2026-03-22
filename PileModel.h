#pragma once
#ifndef PILE_MODEL_H
#define PILE_MODEL_H

#include "cocos2d.h"
#include "CardModel.h"
#include "GlobalEnum.h"
#include <vector>

USING_NS_CC;

class PileModel
{
public:
    PileType m_type;
    std::vector<CardModel> m_cards; // 卡牌列表
    int m_cardCount; // 卡牌数量

    PileModel();
    explicit PileModel(PileType type);

    void addCard(const CardModel& card);
    void removeCard(const CardModel* card);
    CardModel* getTopCard();
    void clear();

    // 序列化/反序列化
    ValueMap serialize() const;
    void deserialize(const ValueMap& json);
};

#endif // PILE_MODEL_H