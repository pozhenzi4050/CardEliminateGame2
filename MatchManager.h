#pragma once
#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H
#include "cocos2d.h"
#include"../Classes/Models/CardModel.h"
#include "GlobalEnum.h"
USING_NS_CC;

class MatchManager
{
public:
    static MatchManager* getInstance();
    static void destroyInstance();

    // G002核心：判断两张牌是否匹配（数字±1）
    bool checkCardMatch(const CardModel* card1, const CardModel* card2);

private:
    MatchManager() = default;
    ~MatchManager() = default;
    static MatchManager* s_instance;
};

#endif // MATCH_MANAGER_H