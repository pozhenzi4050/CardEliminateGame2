#pragma once
#ifndef CARD_FLIP_MANAGER_H
#define CARD_FLIP_MANAGER_H

#include "cocos2d.h"
#include "../Models/PileModel.h"
USING_NS_CC;

class CardFlipManager
{
public:
    static CardFlipManager* getInstance();
    static void destroyInstance();
   // void flipTopCardOfPile(PileModel* mainPile, int pileIndex, int cardsPerPile = 4);
    void flipTopCard(PileModel* mainPile);

private:
    CardFlipManager() = default;
    ~CardFlipManager() = default;
    static CardFlipManager* s_instance;
};

#endif // CARD_FLIP_MANAGER_H