#include "GameManager.h"

// 初始化单例为空
GameManager* GameManager::s_instance = nullptr;

// 获取单例
GameManager* GameManager::getInstance() {
    if (!s_instance) {
        s_instance = new GameManager();
    }
    return s_instance;
}

// 销毁单例
void GameManager::destroyInstance() {
    if (s_instance) {
        delete s_instance;
        s_instance = nullptr;
    }
}

// 构造函数
GameManager::GameManager()
    : m_mainPile(nullptr)
    , m_deckPile(nullptr)
    , m_stockPile(nullptr)
    , m_gameStatus(GAME_READY)
    , m_score(0)
{
}

// 析构函数
GameManager::~GameManager() {
    // 释放牌堆内存
    if (m_mainPile) m_mainPile->release();
    if (m_deckPile) m_deckPile->release();
    if (m_stockPile) m_stockPile->release();
}

// 初始化游戏（核心！）
void GameManager::initGame() {
    // 1. 创建三个牌堆
    m_mainPile = Pile::create(PILE_TYPE_MAIN);
    m_deckPile = Pile::create(PILE_TYPE_DECK);
    m_stockPile = Pile::create(PILE_TYPE_STOCK);
    m_mainPile->retain(); // 手动管理内存
    m_deckPile->retain();
    m_stockPile->retain();

    // 2. 生成52张牌并洗牌
    Vector<Card*> allCards = CardUtils::createStandardDeck();
    CardUtils::shuffleCards(allCards);

    // 3. 分配牌堆（示例：主牌堆30张，底牌堆1张，备用牌堆21张）
    // 主牌堆
    for (int i = 0; i < 30; i++) {
        Card* card = allCards.at(i);
        card->setFaceUp(true); // 主牌堆牌正面朝上
        m_mainPile->addCard(card);
    }
    // 底牌堆（1张）
    Card* deckCard = allCards.at(30);
    deckCard->setFaceUp(true);
    m_deckPile->addCard(deckCard);
    // 备用牌堆（剩余21张）
    for (int i = 31; i < allCards.size(); i++) {
        Card* card = allCards.at(i);
        card->setFaceUp(false); // 备用牌堆牌反面朝上
        m_stockPile->addCard(card);
    }

    // 4. 设置牌堆位置（示例坐标，后续UI层调整）
    m_mainPile->setPilePosition(Vec2(400, 300));
    m_deckPile->setPilePosition(Vec2(200, 100));
    m_stockPile->setPilePosition(Vec2(600, 100));

    // 5. 初始化游戏状态
    m_gameStatus = GAME_PLAYING;
    m_score = 0;

    log("游戏初始化完成！主牌堆：%d张，底牌堆：%d张，备用牌堆：%d张",
        m_mainPile->getCardCount(),
        m_deckPile->getCardCount(),
        m_stockPile->getCardCount());
}

// 消除指定牌
bool GameManager::eliminateCard(Card* card) {
    if (!card || m_gameStatus != GAME_PLAYING) return false;

    // 1. 检查是否能和底牌堆顶部牌匹配
    Card* deckTop = m_deckPile->getTopCard();
    if (!CardUtils::isCardMatch(card, deckTop)) {
        log("牌不匹配，无法消除！");
        return false;
    }

    // 2. 消除牌并加分
    m_mainPile->removeCard(card);
    card->setEliminated(true);
    m_score += 100; // 消除一张加100分

    // 3. 将消除的牌移到底牌堆顶部
    m_deckPile->addCard(card);

    log("消除成功！当前分数：%d", m_score);

    // 4. 检查游戏状态
    checkGameStatus();
    return true;
}

// 从备用牌堆抽牌到底牌堆
void GameManager::drawCardFromStock() {
    if (m_stockPile->getCardCount() == 0 || m_gameStatus != GAME_PLAYING) {
        log("备用牌堆已空，无法抽牌！");
        return;
    }

    // 取出备用牌堆顶部牌
    Card* stockTop = m_stockPile->getTopCard();
    m_stockPile->removeCard(stockTop);
    stockTop->setFaceUp(true);

    // 加入底牌堆
    m_deckPile->addCard(stockTop);

    log("抽牌成功！备用牌堆剩余：%d张", m_stockPile->getCardCount());

    // 检查游戏状态
    checkGameStatus();
}

// 检查游戏胜负
void GameManager::checkGameStatus() {
    // 胜利条件：主牌堆和备用牌堆都空了
    if (m_mainPile->getCardCount() == 0 && m_stockPile->getCardCount() == 0) {
        m_gameStatus = GAME_WIN;
        log("游戏胜利！最终分数：%d", m_score);
        return;
    }

    // 失败条件：主牌堆无匹配牌 + 备用牌堆空
    bool hasMatch = false;
    Card* deckTop = m_deckPile->getTopCard();
    for (auto card : m_mainPile->getCardList()) {
        if (CardUtils::isCardMatch(card, deckTop)) {
            hasMatch = true;
            break;
        }
    }

    if (!hasMatch && m_stockPile->getCardCount() == 0) {
        m_gameStatus = GAME_LOSE;
        log("游戏失败！最终分数：%d", m_score);
    }
}