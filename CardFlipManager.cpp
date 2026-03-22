#include "CardFlipManager.h"

CardFlipManager* CardFlipManager::s_instance = nullptr;

CardFlipManager* CardFlipManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new CardFlipManager();
    }
    return s_instance;
}

void CardFlipManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

void CardFlipManager::flipTopCard(PileModel* mainPile)
{
    if (!mainPile || mainPile->m_cards.empty()) return;

    // 从后往前遍历（从牌堆顶往下找）
    for (auto it = mainPile->m_cards.rbegin(); it != mainPile->m_cards.rend(); ++it)
    {
        if (!it->m_isFront && !it->m_isEliminated)
        {
            it->m_isFront = true; // 翻开
            break; // 只翻最上面的一张
        }
    }
}
//void CardFlipManager::flipTopCardOfPile(PileModel* mainPile, int pileIndex, int cardsPerPile)
//{
//    if (!mainPile || mainPile->m_cards.empty()) return;
//
//    CCLOG("[CardFlipManager] flip%dpile", pileIndex);
//
//    for (auto it = mainPile->m_cards.rbegin(); it != mainPile->m_cards.rend(); ++it)
//    {
//        CardModel& card = *it;
//        // 只翻属于指定堆的牌
//        if (card.m_pileIndex == pileIndex && !card.m_isFront && !card.m_isEliminated)
//        {
//            card.m_isFront = true;
//            CCLOG("[CardFlipManager] filpID=%d，face=%d，index=%d", card.m_id, card.m_face, card.m_pileIndex);
//            break;
//        }
//    }
//}