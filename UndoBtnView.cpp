#include "UndoBtnView.h"


UndoBtnView* UndoBtnView::create()
{
    UndoBtnView* view = new UndoBtnView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool UndoBtnView::init()
{
    CCLOG("UndoBtnView::init star");
    if (!Node::init()) {
        CCLOGERROR("UndoBtnView 父类 Node::init 失败！");
        return false;
    }

    this->setContentSize(Size(200, 80));
    _remainUndoTimes = kMaxUndoTimes;

    // 创建回退按钮
    _undoBtn = Button::create("E:/cocos2d/cocos2d-x-3.17/CardEliminateGame1/Resources/undo.png");
    if (_undoBtn)
    {
        _undoBtn->setContentSize(this->getContentSize());
        _undoBtn->setPosition(this->getContentSize() / 2);
        this->addChild(_undoBtn);
    }

    // 创建次数显示文本
    _countLabel = Label::createWithSystemFont(StringUtils::format("undo(%d)", _remainUndoTimes), "Arial", 24);
    if (_countLabel)
    {
        _countLabel->setPosition(this->getContentSize() / 2);
        _countLabel->setTextColor(Color4B::WHITE);
        this->addChild(_countLabel, 1);
    }

    // 绑定点击事件
    bindClickEvent();
    return true;
}

void UndoBtnView::bindClickEvent()
{
    if (!_undoBtn) return;

    _undoBtn->addClickEventListener([=](Ref* sender) {
        if (onUndoBtnClick && _remainUndoTimes > 0)
        {
            onUndoBtnClick();
        }
        });
}

void UndoBtnView::updateUndoCountShow(int remainTimes)
{
    _remainUndoTimes = MAX(0, remainTimes);
    // 更新文本显示
    if (_countLabel)
    {
        _countLabel->setString(StringUtils::format("UNDO(%d)", _remainUndoTimes));
    }
    // 次数为0时禁用按钮
    if (_undoBtn)
    {
        _undoBtn->setEnabled(_remainUndoTimes > 0);
        _undoBtn->setBright(_remainUndoTimes > 0);
    }
}

void UndoBtnView::resetUndoCount()
{
    updateUndoCountShow(kMaxUndoTimes);
}

int UndoBtnView::getRemainUndoTimes() const
{
    return _remainUndoTimes;
}
void UndoBtnView::setEnabled(bool isEnabled)
{
    if (_undoBtn)
    {
        _undoBtn->setEnabled(isEnabled);
        _undoBtn->setBright(isEnabled);
    }
}
