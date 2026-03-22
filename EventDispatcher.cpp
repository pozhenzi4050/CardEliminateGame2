#include "EventDispatcher.h"

GameEventDispatcher* GameEventDispatcher::s_instance = nullptr;

GameEventDispatcher* GameEventDispatcher::getInstance()
{
    if (!s_instance)
    {
        s_instance = new GameEventDispatcher();
    }
    return s_instance;
}

void GameEventDispatcher::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

GameEventDispatcher::~GameEventDispatcher()
{
    m_listenerMap.clear();
}

void GameEventDispatcher::addEventListener(GameEventType type, void* target, GameEventCallback callback)
{
    if (!target || !callback) return;
    m_listenerMap[type].push_back({ target, callback });
}

void GameEventDispatcher::removeEventListener(GameEventType type, void* target)
{
    if (!target || m_listenerMap.find(type) == m_listenerMap.end()) return;

    auto& listeners = m_listenerMap[type];
    for (auto it = listeners.begin(); it != listeners.end();)
    {
        if (it->first == target)
        {
            it = listeners.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void GameEventDispatcher::removeAllListenersForTarget(void* target)
{
    if (!target) return;

    for (auto& pair : m_listenerMap)
    {
        auto& listeners = pair.second;
        for (auto it = listeners.begin(); it != listeners.end();)
        {
            if (it->first == target)
            {
                it = listeners.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void GameEventDispatcher::dispatchEvent(const GameEvent& event)
{
    if (m_listenerMap.find(event.type) == m_listenerMap.end()) return;

    // 拷贝监听列表，避免回调中注销监听导致迭代器失效
    auto listeners = m_listenerMap[event.type];
    for (auto& listener : listeners)
    {
        if (listener.second)
        {
            listener.second(event);
        }
    }
}