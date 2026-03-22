#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "../Models/CardModel.h"
#include <functional>
#include "../Controllers/EventDispatcher.h"

USING_NS_CC;

class CardView : public Node
{
public:
    static CardView* create(const CardModel* cardModel);
    bool init(const CardModel* cardModel);

    void refreshView();
    const CardModel* getCardModel() const; 

    void playEliminateAnimation(const Vec2& targetWorldPos, std::function<void()> onComplete = nullptr);
    void playErrorShakeAnimation();
    void playMatchSuccessAnimation(const cocos2d::Vec2& targetWorldPos, std::function<void()> onComplete = nullptr);
    ~CardView();

    // 卡牌点击回调
    std::function<void(const CardModel*)> onCardClick;

    void removeAllEventListeners();

private:
    static const Size kCardSize;
    static const float kAnimDuration;

    // 成员变量
    const CardModel* m_cardModel;
    Sprite* m_cardBg;
    Sprite* m_cardBack;
    Sprite* m_numSprite;
    Sprite* m_numSprite1;
    Sprite* m_suitSprite;
    EventListenerTouchOneByOne* m_touchListener;

    // 成员函数
    void loadCardResources();
    void bindTouchEvent();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};

#endif // CARD_VIEW_H