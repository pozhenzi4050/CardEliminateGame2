#pragma once
#ifndef UI_FEEDBACK_VIEW_H
#define UI_FEEDBACK_VIEW_H
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
USING_NS_CC;
using namespace ui;

class UIFeedbackView : public Node
{
public:
    static UIFeedbackView* create();
    bool init() override;
public:



    void playBtnClickEffect(Node* targetNode);

    void playCardClickEffect(Node* targetNode);
    void playScoreTip(const Vec2& worldPos, const std::string& text = "+10");
    void playErrorShakeEffect(Node* targetNode);
    void playEliminateEffect(Node* targetNode, const Vec2& targetWorldPos, std::function<void()> onComplete = nullptr);
    void showGameResultPopup(bool isWin, int finalScore, std::function<void()> onRestartClick);
    void playSound(const std::string& soundName);

private:
    Node* _tipParentNode; // 提示文本的父节点
    // 动画常量
    const float kTipDuration = 0.5f;    // 得分提示动画时长
    const float kAnimDuration = 0.3f;   // 基础动画时长
    const float kCardClickScale = 1.1f; // 卡牌点击缩放比例
};
#endif // UI_FEEDBACK_VIEW_H