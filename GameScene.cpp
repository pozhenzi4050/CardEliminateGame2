#include "GameScene.h"
#include "GlobalEnum.h"
#include <algorithm>
#include"Services/CardService.h"
#include"Services/HistoryService.h"
#include"Controllers/GameController.h"
#include <random>

Scene* GameScene::createScene() {
    auto scene = GameScene::create();
    // 初始化GameController单例，保证全局唯一控制层
    GameController* gameController = GameController::getInstance();
    scene->addChild(gameController);
    CCLOG("[GameScene::createScene] GameController attached to scene");
    return scene;
}
bool GameScene::init() {
    
    if (!Scene::init()) return false;
    CCLOG("[GameScene::init] Scene initialization started");

    // 背景层
    LayerColor* brownBg = LayerColor::create(Color4B(139, 69, 19, 255), 1080, 1500);
    brownBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT); // 锚点左下角
    brownBg->setPosition(0, 580); // 紫色区高度580，所以棕色区从y=580开始
    this->addChild(brownBg, -10); // zOrder设为-10，确保在最底层

    LayerColor* purpleBg = LayerColor::create(Color4B(128, 0, 128, 255), 1080 , 580);
    purpleBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    purpleBg->setPosition(0, 0); // 从左下角(0,0)开始
    this->addChild(purpleBg, -10); // 和棕色层同层级，不重叠);

    //  注册场景级事件
    registerSceneEvents();
    m_historyModel = new HistoryModel();
    HistoryService* historyService = HistoryService::getInstance();
    historyService->setHistoryModel(m_historyModel);
    CCLOG("[GameScene::init] HistoryModel injected to HistoryService");


    CCLOG("[GameScene::init] Scene initialization completed completely!");
    return true;
}

void GameScene::registerSceneEvents() {
    auto dispatcher = GameEventDispatcher::getInstance();
    // 场景级事件（如游戏结束、全局错误）
    dispatcher->addEventListener(EVENT_GAME_STATE_CHANGED, this, [=](const GameEvent& event) {
        GameStatus status = static_cast<GameStatus>(event.userData.at("status").asInt());
        if (status == GAME_WIN) {
            CCLOG("[GameScene] Game win, play victory effect");
            // 场景级胜利逻辑（如播放背景音乐、全局弹窗）
        }
        else if (status == GAME_LOSE) {
            CCLOG("[GameScene] Game lose, play fail effect");
            // 场景级失败逻辑
        }
        });
}



// 析构函数
GameScene::~GameScene() {
    if (GameController::getInstance()) {
        GameController::getInstance()->release();
    }
    CCLOG("[GameScene::~GameScene] Scene destroyed");
}