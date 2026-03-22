#include"MatchManager.h"
MatchManager* MatchManager::s_instance = nullptr;

MatchManager* MatchManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new MatchManager();
    }
    return s_instance;
}

void MatchManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

bool MatchManager::checkCardMatch(const CardModel* card1, const CardModel* card2)
{
    if (!card1 || !card2) return false;

    // 获取牌面数字（枚举值转int：A=0, 2=1, ..., K=12）
    int face1 = static_cast<int>(card1->m_face);
    CCLOG("[card1:%d]", card1->m_face);
    int face2 = static_cast<int>(card2->m_face);
    CCLOG("[card2:%d]", card2->m_face);

    // 核心规则：数字差为1即匹配（无花色限制）
    return abs(face1 - face2) == 1;
}