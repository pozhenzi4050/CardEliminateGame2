#pragma once
#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include "cocos2d.h"
#include "GlobalEnum.h"

USING_NS_CC;

class CardModel
{
public:
    int m_id;
    CardFaceType m_face;
    CardSuitType m_suit;
    Vec2 m_position;
    bool m_isFront;
    PileType m_belongPile;
    bool m_isEliminated;
    int m_pileIndex; //

    CardModel();
    CardModel(int id, CardFaceType face, CardSuitType suit);

    // 序列化/反序列化
    ValueMap serialize() const;
    void deserialize(const ValueMap& json);
};

#endif // CARD_MODEL_H