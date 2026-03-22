#pragma
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "cocos2d.h"

USING_NS_CC;

class MathUtils
{
public:

    static bool isNumDiffOne(int a, int b);


    static Vec2 worldToLocal(Node* parent, const Vec2& worldPos);

    static Vec2 localToWorld(Node* parent, const Vec2& localPos);

    static int clamp(int value, int min, int max);
};

#endif // MATH_UTILS_H