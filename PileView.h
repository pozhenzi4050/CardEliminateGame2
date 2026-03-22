#pragma once
#ifndef PILE_VIEW_H
#define PILE_VIEW_H

#include "cocos2d.h"
#include "../Models/PileModel.h"
#include"./Classes/Controllers/EventDispatcher.h"
#include <functional>
#include <vector>

USING_NS_CC;

class CardView;
class CardModel;

class PileView : public Node
{
public:
    static PileView* create(const PileModel* pileModel);
    bool init(const PileModel* pileModel);
    ~PileView(); // 新增析构函数声明
    PileType getPileType() const;
    // 单张卡牌点击（主牌堆使用，需要传递卡牌）
    std::function<void(const CardModel*, PileType)> onCardClick;
    // 牌堆整体点击（备用牌堆使用，仅传递牌堆类型）
    std::function<void(PileType)> onPileClick;
    void setPileModel(const PileModel* pileModel) {
        m_pileModel = pileModel;
        CCLOG("[PileView] setPileModel called, new model: %p", pileModel);
    }
    // 公有成员
    std::vector<CardView*> m_cardViewList;

    // 公有方法
    void clearAllCards();
    void createCards();

private:
    const PileModel* m_pileModel;
    const float kCardOffsetY = 30.0f;

};

#endif // PILE_VIEW_H