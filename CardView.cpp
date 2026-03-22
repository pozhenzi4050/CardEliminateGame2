#include "CardView.h"
#include "GlobalEnum.h"
#include "cocos2d.h"
const Size CardView::kCardSize = Size(182, 282);
const float CardView::kAnimDuration = 0.3f;

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* view = new CardView();
    if (view && view->init(cardModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel* cardModel)
{
    if (!Node::init()) return false;
    m_cardModel = cardModel;
    if (!m_cardModel) return false;

    GameEventDispatcher::getInstance()->addEventListener(EVENT_UNDO_TRIGGERED, this, [=](const GameEvent& event) {
        // 从事件中获取上一步的状态
        ValueMap lastState = event.userData.at("lastState").asValueMap();

        CCLOG("[CardView] recevice undo ID=%d", m_cardModel ? m_cardModel->m_id : -1);
        });

    GameEventDispatcher::getInstance()->addEventListener(EVENT_CARD_MATCH_SUCCESS, this, [=](const GameEvent& event) {
        int eventCardId = event.userData.at("cardId").asInt();
        if (!m_cardModel || m_cardModel->m_id != eventCardId) {
            return;
        }

        Vec2 targetWorldPos = Vec2(
            event.userData.at("tarX").asFloat(),
            event.userData.at("tarY").asFloat()
        );

        CCLOG("[CardView] Playing animation for card ID=%d", m_cardModel->m_id);
        playEliminateAnimation(targetWorldPos, [=]() {
            CCLOG("[CardView] Animation completed for card ID=%d", m_cardModel->m_id);
            // this->release(); 
            });
        });

    GameEventDispatcher::getInstance()->addEventListener(EVENT_UNDO_TRIGGERED, this, [=](const GameEvent& event) {
        CCLOG("[CardView] Card ID=%d received undo event, wait for rebuild", m_cardModel ? m_cardModel->m_id : -1);
        });

    GameEventDispatcher::getInstance()->addEventListener(EVENT_CARD_MATCH_FAILED, this, [=](const GameEvent& event) {
        int eventCardId = event.userData.at("cardId").asInt();
        if (!m_cardModel || m_cardModel->m_id != eventCardId) {
            return;
        }

        // 执行抖动动画
        this->playErrorShakeAnimation();
        });

    this->setContentSize(kCardSize);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->setPosition(cardModel->m_position);

    m_cardBg = Sprite::create();
    m_cardBg->setTextureRect(Rect(0, 0, kCardSize.width, kCardSize.height));
    m_cardBg->setColor(Color3B(255, 255, 255));
    m_cardBg->setPosition(kCardSize / 2);
    this->addChild(m_cardBg, 0);

    if (m_cardModel->m_belongPile == PILE_BACKUP) {
        // 只有备用牌堆的卡牌才添加黑边
        LayerColor* cardBorder = LayerColor::create(
            Color4B(0, 0, 0, 255),
            kCardSize.width + 4,
            kCardSize.height + 4
        );
        cardBorder->setIgnoreAnchorPointForPosition(false);
        cardBorder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        cardBorder->setPosition(kCardSize / 2);
        this->addChild(cardBorder, -1); // zOrder=-1，在卡牌背景后
    }

    m_cardBack = Sprite::create();
    m_cardBack->setTextureRect(Rect(0, 0, kCardSize.width, kCardSize.height));
    m_cardBack->setColor(Color3B(80, 80, 120));
    m_cardBack->setPosition(kCardSize / 2);
    this->addChild(m_cardBack, 10);

    loadCardResources();
    bindTouchEvent();
    refreshView();

    return true;
}
void CardView::removeAllEventListeners()
{
    GameEventDispatcher::getInstance()->removeAllListenersForTarget(this);
    if (m_touchListener && _eventDispatcher) {
        _eventDispatcher->removeEventListener(m_touchListener);
        m_touchListener = nullptr;
    }
}

CardView::~CardView() {
    // 确保监听全部移除
    removeAllEventListeners();

    // 清空指针，避免野指针
    m_cardModel = nullptr;
    m_cardBg = nullptr;
    m_cardBack = nullptr;
    m_numSprite = nullptr;
    m_numSprite1 = nullptr;
    m_suitSprite = nullptr;
    m_touchListener = nullptr;
}

void CardView::loadCardResources()
{
    if (!m_cardModel || !m_cardBg) {
        CCLOGERROR("[CardView::loadCardResources] Invalid pointer!");
        return;
    }

    std::string bgPath = FileUtils::getInstance()->fullPathForFilename("res/card_general.png");
    Texture2D* bgTexture = Director::getInstance()->getTextureCache()->getTextureForKey(bgPath);
    if (!bgTexture) {
        bgTexture = Director::getInstance()->getTextureCache()->addImage(bgPath);
    }
    if (bgTexture) {
        m_cardBg->setTexture(bgTexture);
        m_cardBg->setTextureRect(Rect(0, 0, bgTexture->getContentSize().width, bgTexture->getContentSize().height));
    }
    else {
        CCLOGERROR("[CardView] Failed to load card bg: %s", bgPath.c_str());
        return;
    }

    // 数组索引越界防护
    std::string faceNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    int faceIndex = m_cardModel->m_face - 1;
    if (faceIndex < 0 || faceIndex >= 13) {
        CCLOGERROR("[CardView] Invalid face value: %d", m_cardModel->m_face);
        return;
    }

    std::string colorStr = (m_cardModel->m_suit == CST_HEARTS || m_cardModel->m_suit == CST_DIAMONDS) ? "red" : "black";
    std::string numPath = StringUtils::format("res/number/big_%s_%s.png", colorStr.c_str(), faceNames[faceIndex].c_str());
    std::string numPath1 = StringUtils::format("res/number/small_%s_%s.png", colorStr.c_str(), faceNames[faceIndex].c_str());

    // 数字
    m_numSprite = Sprite::create(numPath);
    if (m_numSprite) {
        m_numSprite->setPosition(kCardSize.width / 2, kCardSize.height / 2);
        m_numSprite->setScale(0.9f);
        m_cardBg->addChild(m_numSprite, 2);
    }
    else {
        CCLOGERROR("[CardView] Load num sprite failed: %s", numPath.c_str());
    }

    m_numSprite1 = Sprite::create(numPath1);
    if (m_numSprite1) {
        m_numSprite1->setPosition(25, kCardSize.height - 30);
        m_numSprite1->setScale(0.9f);
        m_cardBg->addChild(m_numSprite1, 2);
    }
    else {
        CCLOGERROR("[CardView] Load small num sprite failed: %s", numPath1.c_str());
    }

    // 花色
    std::string suitNames[] = { "club", "diamond", "heart", "spade" };
    int suitIndex = m_cardModel->m_suit;
    if (suitIndex < 0 || suitIndex >= 4) {
        CCLOGERROR("[CardView] Invalid suit value: %d", m_cardModel->m_suit);
        return;
    }
    std::string suitPath = StringUtils::format("res/suits/%s.png", suitNames[suitIndex].c_str());
    m_suitSprite = Sprite::create(suitPath);
    if (m_suitSprite) {
        m_suitSprite->setPosition(100, kCardSize.height - 30);
        m_suitSprite->setScale(0.9f);
        m_cardBg->addChild(m_suitSprite, 2);
    }
    else {
        CCLOGERROR("[CardView] Load suit sprite failed: %s", suitPath.c_str());
    }
}

void CardView::bindTouchEvent()
{
    m_touchListener = EventListenerTouchOneByOne::create();
    if (!m_touchListener) return;

    m_touchListener->setSwallowTouches(true);
    m_touchListener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    m_touchListener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
}

bool CardView::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 touchPos = this->convertToNodeSpace(touch->getLocation());
    return Rect(0, 0, kCardSize.width, kCardSize.height).containsPoint(touchPos);
}

void CardView::onTouchEnded(Touch* touch, Event* event)
{
    if (onCardClick && m_cardModel)
        onCardClick(m_cardModel);
}

void CardView::refreshView()
{
    if (!m_cardBack || !m_cardModel) return;
    m_cardBack->setVisible(!m_cardModel->m_isFront);
}
void CardView::playEliminateAnimation(const Vec2& targetWorldPos, std::function<void()> onComplete)
{
    if (!this->getParent()) {
        CCLOGERROR("[CardView] 无父节点，无法执行动画！");
        if (onComplete) onComplete();
        return;
    }

    // 转换世界坐标到本地坐标（核心：移动动画的关键）
    Vec2 targetLocalPos = this->getParent()->convertToNodeSpace(targetWorldPos);
    CCLOG("[CardView] ID=%d moveing：(%f,%f) → (%f,%f)",
        m_cardModel->m_id, this->getPositionX(), this->getPositionY(),
        targetLocalPos.x, targetLocalPos.y);

    this->runAction(Sequence::create(
        // 移动动画：移动到目标位置，轻微放大（视觉反馈）
        Spawn::create(
            MoveTo::create(1.5f, targetLocalPos),
            ScaleTo::create(1.5f, 1.1f),
            nullptr
        ),
        ScaleTo::create(0.1f, 1.0f),

        CallFunc::create([this, onComplete]() {
            CCLOG("[CardView] ID=%d move star", m_cardModel->m_id);
            if (this->getParent()) {
            }
            if (onComplete) onComplete();
            }),
        nullptr
    ));
}
void CardView::playErrorShakeAnimation()
{
    Vec2 originPos = this->getPosition();
    this->runAction(Sequence::create(
        MoveBy::create(0.05f, Vec2(-10, 0)),
        MoveBy::create(0.05f, Vec2(20, 0)),
        MoveBy::create(0.05f, Vec2(-15, 0)),
        MoveBy::create(0.05f, Vec2(10, 0)),
        MoveTo::create(0.05f, originPos),
        nullptr
    ));
}
void CardView::playMatchSuccessAnimation(const cocos2d::Vec2& targetWorldPos, std::function<void()> onComplete)
{
    if (!this->getParent()) {
        CCLOGERROR("[CardView] 无父节点，无法执行移动动画！");
        if (onComplete) onComplete();
        return;
    }

    // 转换世界坐标到父节点的本地坐标
    Vec2 targetLocalPos = this->getParent()->convertToNodeSpace(targetWorldPos);
    CCLOG("[CardView] ID=%d move star：from(%f,%f)to(%f,%f)",
        m_cardModel->m_id, this->getPositionX(), this->getPositionY(),
        targetLocalPos.x, targetLocalPos.y);

    // 组合动画：移动+缩放+渐变
    this->runAction(Sequence::create(
        Spawn::create(
            MoveTo::create(kAnimDuration, targetLocalPos), // 核心移动动画
            ScaleTo::create(kAnimDuration, 1.0f),          // 缩放至正常大小
            FadeIn::create(kAnimDuration),                 // 渐变显示
            nullptr
        ),
        CallFunc::create([this, onComplete]() {
            CCLOG("[CardView] ID=%d movesuccesss", m_cardModel->m_id);
            // 动画完成后刷新视图（比如显示正面）
            this->refreshView();
            if (onComplete) onComplete();
            }),
        nullptr
    ));
}
