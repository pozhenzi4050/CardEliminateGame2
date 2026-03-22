#pragma once
#ifndef UNDO_BTN_VIEW_H
#define UNDO_BTN_VIEW_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
#include"./Classes/Controllers/EventDispatcher.h"
USING_NS_CC;
using namespace ui;


class UndoBtnView : public Node
{
public:
    static UndoBtnView* create();
    bool init() override;
    void updateUndoCountShow(int remainTimes);
    //重置回退次数（游戏重新开始时调用）
    void resetUndoCount();


    // 获取当前剩余回退次数
    int getRemainUndoTimes() const;

    // 回退按钮点击事件回调
    std::function<void()> onUndoBtnClick;

    void setEnabled(bool isEnabled);

private:
    Button* _undoBtn; // 回退按钮
    Label* _countLabel; // 次数显示文本
    const int kMaxUndoTimes = 5; // 最大回退次数
    int _remainUndoTimes; // 当前剩余回退次数

    void bindClickEvent();
};

#endif // UNDO_BTN_VIEW_H