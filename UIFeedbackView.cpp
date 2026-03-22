#include "UIFeedbackView.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;

UIFeedbackView* UIFeedbackView::create()
{
    UIFeedbackView* view = new UIFeedbackView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool UIFeedbackView::init()
{
    if (!Node::init())
    {
        return false;
    }
    this->setContentSize(Size(1080, 2080));
    // 提示文本父节点，覆盖整个屏幕
    _tipParentNode = Node::create();
    _tipParentNode->setContentSize(this->getContentSize());
    this->addChild(_tipParentNode, 100);
    return true;
}

void UIFeedbackView::playBtnClickEffect(Node* targetNode)
{

}

void UIFeedbackView::playCardClickEffect(Node* targetNode)
{
    if (!targetNode) return;
    // 播放卡牌点击音效
    playSound("card_click");
    // 卡牌选中缩放+轻微上浮
    targetNode->runAction(Sequence::create(
        Spawn::create(
            ScaleTo::create(0.1f, kCardClickScale),
            MoveBy::create(0.1f, Vec2(0, 10)),
            nullptr
        ),
        Spawn::create(
            ScaleTo::create(0.1f, 1.0f),
            MoveBy::create(0.1f, Vec2(0, -10)),
            nullptr
        ),
        nullptr
    ));
}


void UIFeedbackView::playErrorShakeEffect(Node* targetNode)
{


}

void UIFeedbackView::playEliminateEffect(Node* targetNode, const Vec2& targetWorldPos, std::function<void()> onComplete)
{
  
}

void UIFeedbackView::showGameResultPopup(bool isWin, int finalScore, std::function<void()> onRestartClick)
{

}
void UIFeedbackView::playSound(const std::string& soundName)
{

}

void UIFeedbackView::playScoreTip(const Vec2& worldPos, const std::string& text)
{
 
}
