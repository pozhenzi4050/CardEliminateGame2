#pragma once
#ifndef UNDO_CONTROLLER_H
#define UNDO_CONTROLLER_H
#include "cocos2d.h"
#include"GlobalEnum.h"
#include"Views/UndoBtnView.h"
#include "GameController.h"
#include "EventDispatcher.h"
#include "Managers/HistoryManager.h"
#include "Models/GameModel.h"
#include"Services/HistoryService.h"
USING_NS_CC;

class UndoController : public Node
{
public:
    static UndoController* create();
    bool init() override;
    void registerUndoEvent(); // 注册回退事件监听
    void handleUndoOperation(); // 处理回退逻辑
private:
    HistoryManager* m_historyManager;
    const int kMaxUndoTimes = MAX_UNDO_TIMES; // 与全局常量保持一致

};

#endif // UNDO_CONTROLLER_H