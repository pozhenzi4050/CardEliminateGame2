#pragma once
#ifndef GLOBAL_ENUM_H
#define GLOBAL_ENUM_H

// 纸牌花色
enum CardSuitType
{
    CST_CLUBS,    // 梅花
    CST_DIAMONDS, // 方块
    CST_HEARTS,   // 红桃
    CST_SPADES,   // 黑桃
    CST_NONE      // 无
};


enum CardFaceType
{
    CFT_NONE = 0, 
    CFT_A = 1,
    CFT_TWO = 2,
    CFT_THREE = 3,
    CFT_FOUR = 4,
    CFT_FIVE = 5,
    CFT_SIX = 6,
    CFT_SEVEN = 7,
    CFT_EIGHT = 8,
    CFT_NINE = 9,
    CFT_TEN = 10,
    CFT_J = 11,
    CFT_Q = 12,
    CFT_K = 13
};

// 牌堆类型
enum PileType
{
    PILE_MAIN,    // 主牌堆
    PILE_BOTTOM,  // 底牌堆
    PILE_BACKUP,  // 备用牌堆
    PILE_NONE     //
};

enum GameStatus
{
    GAME_PLAYING, // 游戏中
    GAME_WIN,     // 游戏胜利
    GAME_LOSE,    // 游戏失败
    GAME_PAUSE    // 游戏暂停（扩展）
};

// 操作类型（为M004历史记录准备）
enum OperateType
{
    OP_ELIMINATE, // 消除纸牌
    OP_FLIP,      // 翻备用牌
    OP_UNDO,       // 回退操作（扩展）
    OP_MATCH
};
// 全局配置常量
const int MAX_UNDO_TIMES = 5;

#endif // GLOBAL_ENUM_H