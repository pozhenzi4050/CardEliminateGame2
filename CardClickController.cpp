#include "CardClickController.h"
#include "GameController.h"
#include "EventDispatcher.h"
#include "Managers/MatchManager.h"
#include "Managers/PileManager.h"
#include "Managers/HistoryManager.h"
#include"Managers/GameStateManager.h"
#include "Views/PileView.h"
#include "Views/UIFeedbackView.h"
#include "Models/GameModel.h"
#include"Services/HistoryService.h"
#include"Services/CardService.h"

CardClickController* CardClickController::create()
{
    CardClickController* controller = new CardClickController();
    if (controller && controller->init())
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool CardClickController::init()
{
    if (!Node::init()) return false;

    m_matchManager = MatchManager::getInstance();
    m_pileManager = PileManager::getInstance();
    m_historyManager = HistoryManager::getInstance();
    m_uiFeedbackView = UIFeedbackView::create();

    registerCardClickEvent();
    return true;
}

void CardClickController::registerCardClickEvent()
{
    auto gameController = GameController::getInstance();
    if (!gameController) return;

    // 监听主牌堆/底牌堆/备用牌堆的纸牌点击事件
    GameEventDispatcher::getInstance()->addEventListener(EVENT_CARD_CLICKED, this, [=](const GameEvent& event) {
        if (!m_isEnabled) return;

        PileType pileType = static_cast<PileType>(event.userData.at("pileType").asInt());
        const CardModel* card = reinterpret_cast<const CardModel*>(event.userData.at("cardPtr").asInt());

        // 主牌堆纸牌点击：走匹配消除逻辑
        if (pileType == PILE_MAIN)
        {
            handleMainCardClick(card);
        }
        // 备用牌堆点击：翻牌逻辑
        else if (pileType == PILE_BACKUP)
        {
            handleBackupPileClick();
        }
        });
}

// 处理备用牌堆点击
// CardClickController.cpp
void CardClickController::handleBackupPileClick()
{
    GameModel* gameModel = GameController::getInstance()->getGameModel();
    if (!gameModel || gameModel->m_status != GAME_PLAYING) {
        CCLOG("[CardClickController] Game not in playing status!");
        return;
    }
    if (gameModel->m_backupPile.m_cards.empty()) {
        CCLOG("[CardClickController] Backup pile is empty!");
        GameEventDispatcher::getInstance()->dispatchEvent({ EVENT_CARD_MATCH_FAILED, {} });
        return;
    }

    // 操作前先记录历史！
    m_historyManager->addRecord(OP_FLIP, *gameModel);

    // 执行翻牌逻辑
    bool flipSuccess = CardService::getInstance()->flipBackupCardToBottom(gameModel);

    if (flipSuccess) {
        GameEventDispatcher::getInstance()->dispatchEvent({ EVENT_PILE_UPDATED, {} });
    }
    else {
        CCLOG("[CardClickController] Flip backup card failed!");
        GameEventDispatcher::getInstance()->dispatchEvent({ EVENT_CARD_MATCH_FAILED, {} });
    }
}// 处理桌面牌点击
void CardClickController::handleMainCardClick(const CardModel* cardModel)
{
    if (!cardModel || !cardModel->m_isFront) return; // 删除消除标记判断
    GameModel* gameModel = GameController::getInstance()->getGameModel();
    if (!gameModel || gameModel->m_status != GAME_PLAYING) return;

    m_historyManager->addRecord(OP_MATCH, *gameModel);
    bool matchSuccess = CardService::getInstance()->matchMainCardToBottom(gameModel, cardModel);

    if (matchSuccess)
    {
      
        // 3. 通知View刷新
        GameEventDispatcher::getInstance()->dispatchEvent({ EVENT_PILE_UPDATED, {} });
    }
    else {
        CCLOG("[CardClickController] No match");
        GameEventDispatcher::getInstance()->dispatchEvent({
           EVENT_CARD_MATCH_FAILED,
           {{"cardId", Value(cardModel->m_id)}} 
            });
    }
}