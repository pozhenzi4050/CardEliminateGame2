#pragma once
#ifndef CARD_CLICK_CONTROLLER_H
#define CARD_CLICK_CONTROLLER_H
#include "cocos2d.h"
#include "../Models/CardModel.h"
#include "GlobalEnum.h"
USING_NS_CC;

// 前置声明
class MatchManager;
class PileManager;
class HistoryManager;
class UIFeedbackView;

class CardClickController : public Node
{
public:
    static CardClickController* create();
    bool init() override;

    // 交互开关
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
    // 私有方法
    void registerCardClickEvent(); // 注册纸牌点击监听
    void handleMainCardClick(const CardModel* cardModel); // 处理主牌堆纸牌点击
    void handleBackupPileClick(); // 处理备用牌堆点击

private:
    bool m_isEnabled = true;
    // Manager引用
    MatchManager* m_matchManager;
    PileManager* m_pileManager;
    HistoryManager* m_historyManager;
    UIFeedbackView* m_uiFeedbackView;


};

#endif // CARD_CLICK_CONTROLLER_H