#include "PileView.h"
#include "CardView.h"
#include "../Models/CardModel.h"
#include <cstdint> // 解决指针转整数的截断问题
#include "GlobalEnum.h"
#include"cocos2d.h"

PileView* PileView::create(const PileModel* pileModel)
{
    PileView* view = new PileView();
    if (view && view->init(pileModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

// PileView::init
bool PileView::init(const PileModel* pileModel)
{
    if (!Node::init()) return false;
    m_pileModel = pileModel;


    // 每个牌堆单独设置尺寸，适配1080*2080竖屏
    switch (m_pileModel->m_type)
    {
    case PILE_MAIN:
        // 主牌堆：占满屏幕上半部分
        this->setContentSize(Size(1080, 1400));
        break;
    case PILE_BOTTOM:
    case PILE_BACKUP:
        // 备用/底牌堆：固定小尺寸，刚好放一叠牌
        this->setContentSize(Size(200, 200));
        break;
    default:
        break;
    }

    // 锚点固定为左下角，彻底避免坐标系混乱
    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    createCards();
    //CCLOG("[PileView::init] PileView init completed");
    return true;
}


void PileView::createCards()
{
    if (!m_pileModel) {
        CCLOGERROR("[PileView::createCards] ERROR: m_pileModel is null!");
        return;
    }
    clearAllCards();

    const float kBackupPileOffsetX = 6.0f;
    const float kMainPileOffsetInPile = 40.0f; // 堆内卡牌水平偏移
    const float kMainPileGapX = 350.0f; // 堆与堆之间的水平间距
    const int kMaxPileIndex = 5; // 最大堆号（0-5共6堆）

    Size pileSize = this->getContentSize();
    int totalCardCount = m_pileModel->m_cards.size();

    // ========== 主牌堆：先按m_pileIndex分组 ==========
    std::unordered_map<int, std::vector<const CardModel*>> pileCardMap;
    if (m_pileModel->m_type == PILE_MAIN)
    {
        for (int i = 0; i < totalCardCount; i++)
        {
            const CardModel& card = m_pileModel->m_cards[i];
            // 如果pileIndex超出范围，强制修正到0-5
            int safePileIndex = MAX(0, MIN(card.m_pileIndex, kMaxPileIndex));
            pileCardMap[safePileIndex].push_back(&card);
        }
    }

    for (int i = 0; i < totalCardCount; i++)
    {
        const CardModel& card = m_pileModel->m_cards[i];
        CardView* cardView = CardView::create(&card);
        if (!cardView) {
            CCLOGERROR("[PileView::createCards] Failed to create CardView! index=%d, face=%d", i, card.m_face);
            continue;
        }

        this->addChild(cardView);
        m_cardViewList.push_back(cardView);
        cardView->setVisible(true);

        Vec2 finalPos = Vec2::ZERO;

        // 备用牌堆
        if (m_pileModel->m_type == PILE_BACKUP)
        {
            finalPos.x = 60 + i * kBackupPileOffsetX;
            finalPos.y = pileSize.height / 2;
            cardView->setLocalZOrder(100 + i);
        }
        // 底牌堆
        else if (m_pileModel->m_type == PILE_BOTTOM)
        {
            finalPos.x = 60;
            finalPos.y = pileSize.height / 2;
            cardView->setLocalZOrder(i == totalCardCount - 1 ? totalCardCount : i);
        }
        // 主牌堆布局（三行6堆：上1、中3、下2）
        else if (m_pileModel->m_type == PILE_MAIN)
        {
            int pileIndex = MAX(0, MIN(card.m_pileIndex, kMaxPileIndex));
            auto& pileCards = pileCardMap[pileIndex];

            // 找卡牌在堆内的索引
            int cardIndexInPile = 0;
            for (int j = 0; j < pileCards.size(); j++)
            {
                if (pileCards[j]->m_id == card.m_id)
                {
                    cardIndexInPile = j;
                    break;
                }
            }

            float startX = 0.0f;
            float startY = 0.0f;

            // 三行布局逻辑
            switch (pileIndex)
            {
            case 0: // 第一行：1堆（居中）
                startX = pileSize.width / 2;
                startY = pileSize.height - 800.0f;
                break;
            case 1: // 第二行第1堆
            case 2: // 第二行第2堆
            case 3: // 第二行第3堆
            {
                int row2PileIndex = pileIndex - 1;
                float totalWidthRow2 = 2 * kMainPileGapX;
                startX = (pileSize.width - totalWidthRow2) / 2 + row2PileIndex * kMainPileGapX;
                startY = pileSize.height - 1200.0f;
                break;
            }
            case 4: // 第三行第1堆
            case 5: // 第三行第2堆
            {
                int row3PileIndex = pileIndex - 4;
                float totalWidthRow3 = 1 * kMainPileGapX;
                startX = (pileSize.width - totalWidthRow3) / 2 + row3PileIndex * kMainPileGapX;
                startY = pileSize.height - 1600.0f;
                break;
            }
            }

            // 计算最终位置
            finalPos.x = startX + cardIndexInPile * kMainPileOffsetInPile;
            finalPos.y = startY;
            cardView->setLocalZOrder(pileIndex * 100 + cardIndexInPile);
        }

        cardView->setPosition(finalPos);
        // 绑定点击事件
        PileType currentPileType = m_pileModel->m_type;
        cardView->onCardClick = [this, currentPileType](const CardModel* card) {
            if (!this || !card) return;
            if (this->onCardClick) {
                this->onCardClick(card, currentPileType);
            }
            GameEvent event;
            event.type = EVENT_CARD_CLICKED;
            event.userData["cardPtr"] = Value((intptr_t)card);
            event.userData["pileType"] = Value((int)currentPileType);
            GameEventDispatcher::getInstance()->dispatchEvent(event);
            };
    }
}
PileView::~PileView()
{
    // 析构时强制清空所有卡牌的回调，彻底杜绝回调残留触发
    clearAllCards();
}

void PileView::clearAllCards()
{
    for (auto cardView : m_cardViewList) {
        cardView->onCardClick = nullptr; 
        cardView->removeFromParent(); 
    }
    m_cardViewList.clear();
}
PileType PileView::getPileType() const
{
    return m_pileModel ? m_pileModel->m_type : PILE_NONE;
}