#pragma once
#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H
#include "cocos2d.h"
#include "GlobalEnum.h"
#include <functional>
#include <vector>
#include <string>
USING_NS_CC;

// 全局游戏事件类型定义（可扩展）
enum GameEventType
{
    // 纸牌操作事件
    EVENT_CARD_CLICKED,        // 纸牌被点击
    EVENT_CARD_MATCH_SUCCESS,   // 纸牌匹配成功
    EVENT_CARD_MATCH_FAILED,    // 纸牌匹配失败
    EVENT_CARD_ELIMINATED,      // 纸牌被消除
    // 牌堆事件
    EVENT_PILE_UPDATED,         // 牌堆数据更新
    EVENT_BACKUP_CARD_FLIPPED,  // 备用牌被翻开
    // 游戏状态事件
    EVENT_GAME_STATE_CHANGED,   // 游戏状态变更
    EVENT_SCORE_CHANGED,        // 分数变更
    // 回退操作事件
    EVENT_UNDO_TRIGGERED,       // 回退操作触发
    EVENT_UNDO_COMPLETED,       // 回退操作完成
    EVENT_UNDO_COUNT_CHANGED,   // 回退次数变更
    // 通用事件
    EVENT_GAME_RESTART,         // 游戏重启
};

// 事件数据结构体，统一事件参数格式
struct GameEvent
{
    GameEventType type;
    ValueMap userData; // 自定义参数，兼容所有数据类型
};

// 事件回调函数类型
using GameEventCallback = std::function<void(const GameEvent&)>;

class GameEventDispatcher
{
public:
    // 全局单例
    static GameEventDispatcher* getInstance();
    static void destroyInstance();
    void addEventListener(GameEventType type, void* target, GameEventCallback callback);

    void removeEventListener(GameEventType type, void* target);
 
    void removeAllListenersForTarget(void* target);
  
    void dispatchEvent(const GameEvent& event);

private:
    GameEventDispatcher() = default;
    ~GameEventDispatcher();
    static GameEventDispatcher* s_instance;

    // 事件监听存储：key=事件类型，value=监听列表（目标+回调）
    std::unordered_map<GameEventType, std::vector<std::pair<void*, GameEventCallback>>> m_listenerMap;
};

#endif // EVENT_DISPATCHER_H