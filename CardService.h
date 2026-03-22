#pragma once
#include "GlobalEnum.h"
#include"Models/CardModel.h"
#include"Models/GameModel.h"
#include"Managers/MatchManager.h"
#include"Managers/PileManager.h"
#include"Managers/CardFlipManager.h"

// 卡牌业务服务（翻牌替换+匹配替换的业务逻辑）
class CardService
{
public:
    static CardService* getInstance();
    static void destroyInstance();

    // 需求1：手牌区翻牌替换（备用牌→手牌区顶部，平移替换）
    bool flipBackupCardToBottom(GameModel* gameModel);

    // 需求2：桌面牌匹配替换手牌顶部（点数差1匹配，平移替换）
    bool matchMainCardToBottom(GameModel* gameModel, const CardModel* targetCard);

private:
    CardService();
    ~CardService();
    static CardService* s_instance;

    // 内部工具：获取备用牌顶牌原始位置（适配视图坐标）
    Vec2 getBackupCardOriginPos(GameModel* gameModel);

    MatchManager* m_matchManager;
    PileManager* m_pileManager;
};