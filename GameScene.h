#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include"../Classes/Models/CardModel.h"
#include "GlobalEnum.h"
#include"../Classes/Views/UIFeedbackView.h"
#include"../Classes/Models/HistoryModel.h"
#include"../Classes/Models/PileModel.h"
#include"../Classes/Views/CardView.h"
#include"../Classes/Views/PileView.h"
#include"../Classes\Controllers\UndoController.h"
#include"./Classes/Views/UndoBtnView.h"
#include"Controllers/CardClickController.h"
USING_NS_CC;


class GameScene : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	virtual bool init() override;

    void registerSceneEvents();

	// 宏定义（Cocos2d-x 反射需要）
	CREATE_FUNC(GameScene);
	~GameScene();



private:
    // 核心数据模型
    GameModel* m_gameModel = nullptr;
    HistoryModel* m_historyModel = nullptr;

    // 核心视图组件
    UIFeedbackView* m_feedbackView = nullptr;
    UndoBtnView* m_undoBtnView = nullptr;
    UndoController* m_undoController = nullptr;
    CardClickController* m_cardClickController = nullptr;
    PileView* m_mainPileView = nullptr;
    PileView* m_bottomPileView = nullptr;
    PileView* m_backupPileView = nullptr;

    // 回退事件监听
    void addUndoEventListenr();
    // 生成一副标准52张纸牌
    void generateStandardCards();
     void onMainCardClick(const CardModel* clickCard, PileType pileType);
    void onBackupPileClick(PileType pileType);
    // 洗牌算法
    void shuffleCards(std::vector<CardModel>& cards);
};

#endif // GAME_SCENE_H