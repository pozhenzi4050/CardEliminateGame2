#include "PileManager.h"
#include "CardFlipManager.h"
#include <algorithm>
#include <random>

// 静态单例定义（必须写在cpp里）
PileManager* PileManager::s_instance = nullptr;

PileManager* PileManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new PileManager();
    }
    return s_instance;
}
// PileManager.cpp
void PileManager::initPiles(GameModel* gameModel) {
    if (!gameModel) return;

    // 复用原generateStandardCards()的代码
    std::vector<CardModel> allCards;
    int cardId = 0;
    for (int suit = CST_CLUBS; suit <= CST_SPADES; suit++) {
        for (int face = CFT_A; face <= CFT_K; face++) {
            CardModel card(cardId++, (CardFaceType)face, (CardSuitType)suit);
            card.m_position = Vec2(540, 1040);
            card.m_isFront = false;
            allCards.push_back(card);
        }
    }
    // 洗牌
    shuffleCards(allCards);

    // 分堆逻辑（复用原代码）
    CardModel bottomCard = allCards.back();
    allCards.pop_back();
    bottomCard.m_isFront = true;
    bottomCard.m_belongPile = PILE_BOTTOM;
    gameModel->m_bottomPile.addCard(bottomCard);

    int mainPileCount = 24;
    const int kCardsPerPile = 4; // 每堆4张
    for (int i = 0; i < mainPileCount; i++)
    {
        CardModel card = allCards.back();
        allCards.pop_back();
        card.m_belongPile = PILE_MAIN;
        card.m_isFront = true;
        // 给卡牌标记堆号
        card.m_pileIndex = i / kCardsPerPile;
        gameModel->m_mainPile.addCard(card);
    }
    for (auto& backupCard : allCards) {
        backupCard.m_belongPile = PILE_BACKUP;
        gameModel->m_backupPile.addCard(backupCard);
    }
    gameModel->updatePileRemain();

    CCLOG("[PileManager::initPiles] Card distribution completed");
}

// 洗牌函数移到PileManager中
void PileManager::shuffleCards(std::vector<CardModel>& cards) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

void PileManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

PileManager::PileManager()
{
    m_cardFlipManager = CardFlipManager::getInstance();
}

// 消除主牌堆的牌
void PileManager::eliminateCard(GameModel* gameModel, const CardModel* cardModel)
{
    if (!gameModel || !cardModel) return;

    // 1. 从主牌堆移除该牌
    gameModel->m_mainPile.removeCard(cardModel);

    // 2. 把该牌添加到底牌堆（作为新的顶牌）
    CardModel newBottomCard = *cardModel;
    newBottomCard.m_belongPile = PILE_BOTTOM;
    newBottomCard.m_isFront = true;
    newBottomCard.m_isEliminated = true;
    gameModel->m_bottomPile.addCard(newBottomCard);

    // 3. 翻开主牌堆下一张
    m_cardFlipManager->flipTopCard(&gameModel->m_mainPile);

    // 4. 同步牌堆数量
    gameModel->updatePileRemain();
}

// 翻备用牌
bool PileManager::flipBackupCard(GameModel* gameModel)
{
    if (!gameModel || gameModel->m_backupPile.m_cards.empty()) return false;

    // 1. 从备用牌堆取顶牌
    CardModel* backupTop = gameModel->m_backupPile.getTopCard();
    if (!backupTop) return false;

    // 2. 移到底牌堆
    CardModel newBottomCard = *backupTop;
    newBottomCard.m_belongPile = PILE_BOTTOM;
    newBottomCard.m_isFront = true;
    gameModel->m_bottomPile.addCard(newBottomCard);

    // 3. 从备用牌堆移除
    gameModel->m_backupPile.m_cards.pop_back();

    // 4. 同步牌堆数量
    gameModel->updatePileRemain();
    return true;
}