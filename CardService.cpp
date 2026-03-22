#include "CardService.h"
#include "./Controllers/EventDispatcher.h"

CardService* CardService::s_instance = nullptr;

CardService::CardService()
{
    // 初始化依赖的Manager（暂时保留，后续迁移）
    m_matchManager = MatchManager::getInstance();
    m_pileManager = PileManager::getInstance();
}

CardService::~CardService()
{
    // 空析构，保留Manager生命周期
}

CardService* CardService::getInstance()
{
    if (!s_instance)
    {
        s_instance = new CardService();
    }
    return s_instance;
}

void CardService::destroyInstance()
{
    if (s_instance)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}

// CardService.cpp
bool CardService::flipBackupCardToBottom(GameModel* gameModel)
{
    if (!gameModel) {
        CCLOGERROR("[CardService] GameModel is null for backup flip!");
        return false;
    }
    if (gameModel->m_backupPile.m_cards.empty()) {
        CCLOGERROR("[CardService] Backup pile is empty!");
        return false;
    }

    // 取备用牌堆顶牌
    CardModel backupTopCard = gameModel->m_backupPile.m_cards.back();
    gameModel->m_backupPile.m_cards.pop_back(); // 从备用牌堆移除

    // 初始化顶牌属性
    backupTopCard.m_isFront = true;          // 翻牌为正面
    backupTopCard.m_belongPile = PILE_BOTTOM;// 归属底牌区
    backupTopCard.m_isEliminated = false;    // 清除消除标记（避免残留）

    //  计算底牌区位置
    CardModel* bottomTopCard = gameModel->m_bottomPile.getTopCard();
    if (bottomTopCard) {
        // 沿底牌区已有偏移规则，避免位置重叠
        backupTopCard.m_position = bottomTopCard->m_position + Vec2(6.0f, 0.0f);
    }
    else {
        // 底牌区为空时的默认位置
        backupTopCard.m_position = Vec2(640.0f, 100.0f);
    }

    // 4. 加入底牌堆
    gameModel->m_bottomPile.addCard(backupTopCard);
    gameModel->updatePileRemain(); // 更新牌堆计数

    // 5. 分发事件
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_CARD_MATCH_SUCCESS,
        {
            {"cardId", Value(backupTopCard.m_id)},
            {"oriX", Value(60.0f)},          // 备用牌区初始X
            {"oriY", Value(100.0f)},        
            {"tarX", Value(backupTopCard.m_position.x)},
            {"tarY", Value(backupTopCard.m_position.y)}
        }
        });

    CCLOG("[CardService] Flip backup card(id=%d, face=%d) to bottom pile success!",
        backupTopCard.m_id, backupTopCard.m_face);
    return true;
}

bool CardService::matchMainCardToBottom(GameModel* gameModel, const CardModel* targetCard)
{
    if (!gameModel || !targetCard) return false;
    // 1. 获取底牌堆顶部牌
    CardModel* bottomTop = gameModel->m_bottomPile.getTopCard();
    if (!bottomTop) return false;

    CCLOG("ID=%d, face=%d", targetCard->m_id, targetCard->m_face);
    CCLOG("ID=%d, face=%d", bottomTop->m_id, bottomTop->m_face);

    // 2. 匹配规则校验（点数差1）
    bool isMatch = m_matchManager->checkCardMatch(targetCard, bottomTop);
    if (!isMatch)
    {
        CCLOG("[match filed] ");
        return false;
    }

    Vec2 oriPos = targetCard->m_position;
    Vec2 targetPos = bottomTop->m_position;
    int targetCardId = targetCard->m_id;

    const int kCardsPerPile = 4; 
    int cardIndex = -1;

    for (int i = 0; i < gameModel->m_mainPile.m_cards.size(); ++i)
    {
        if (gameModel->m_mainPile.m_cards[i].m_id == targetCard->m_id)
        {
            cardIndex = i;
            break;
        }
    }
    // 修复：校验cardIndex合法性
    if (cardIndex == -1) {
        CCLOGERROR("[CardService] targetCard(id=%d) not found in mainPile!", targetCard->m_id);
        return false;
    }
    int pileIndex = cardIndex / kCardsPerPile; // 计算堆号
    // 校验pileIndex范围（中间牌堆假设是0-5）
    if (pileIndex < 0 || pileIndex >= 6) {
        CCLOGERROR("[CardService] pileIndex(%d) out of range!", pileIndex);
        return false;
    }
    CCLOG("[CardService] cardindex=%d，pileIndex=%d", cardIndex, pileIndex);

    CardModel newBottomCard = *targetCard;
    newBottomCard.m_belongPile = PILE_BOTTOM;
    newBottomCard.m_isFront = true;
    newBottomCard.m_position = targetPos;
    gameModel->m_bottomPile.addCard(newBottomCard);
    gameModel->m_mainPile.removeCard(targetCard);
    gameModel->updatePileRemain();
    
    GameEventDispatcher::getInstance()->dispatchEvent({
        EVENT_CARD_MATCH_SUCCESS,
        {
            {"cardId", Value(targetCardId)}, // 用 cardId，避免野指针
            //{"cardPtr", Value((intptr_t)targetCard)}, // 保留 cardPtr，兼容现有代码
            {"oriX", Value(oriPos.x)},
            {"oriY", Value(oriPos.y)},
            {"tarX", Value(targetPos.x)},
            {"tarY", Value(targetPos.y)}
        }
        });
    GameEventDispatcher::getInstance()->dispatchEvent({ EVENT_PILE_UPDATED, {} });

    return true;
}


// 内部工具：获取备用牌顶牌原始位置
Vec2 CardService::getBackupCardOriginPos(GameModel* gameModel)
{
    int cardCount = gameModel->m_backupPile.m_cards.size();
    return Vec2(60 + (cardCount - 1) * 6.0f, 100); // 和View层坐标一致
}