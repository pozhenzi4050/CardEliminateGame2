#include "MathUtils.h"

bool MathUtils::isNumDiffOne(int a, int b)
{
    return abs(a - b) == 1;
}

Vec2 MathUtils::worldToLocal(Node* parent, const Vec2& worldPos)
{
    if (!parent) return worldPos;
    return parent->convertToNodeSpaceAR(worldPos);
}

Vec2 MathUtils::localToWorld(Node* parent, const Vec2& localPos)
{
    if (!parent) return localPos;
    return parent->convertToWorldSpaceAR(localPos);
}

int MathUtils::clamp(int value, int min, int max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}