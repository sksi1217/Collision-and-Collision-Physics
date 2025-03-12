// System/CollisionChecks.cpp
#include "CollisionChecks.h"
#include <algorithm>

namespace CollisionChecks {
    bool CheckCircleCircle(
        ColliderCircleComponent* a,
        ColliderCircleComponent* b)
    {
        float dx = a->CircleCollider.x - b->CircleCollider.x;
        float dy = a->CircleCollider.y - b->CircleCollider.y;
        float r = a->CircleRadius + b->CircleRadius;
        return (dx * dx + dy * dy) < (r * r);
    }

    bool CheckBoxBox(
        ColliderBoxComponent* a,
        ColliderBoxComponent* b)
    {
        return !(a->ColliderBox.x + a->ColliderBox.w < b->ColliderBox.x ||
            a->ColliderBox.x > b->ColliderBox.x + b->ColliderBox.w ||
            a->ColliderBox.y + a->ColliderBox.h < b->ColliderBox.y ||
            a->ColliderBox.y > b->ColliderBox.y + b->ColliderBox.h);
    }

    bool CheckCircleBox(
        ColliderCircleComponent* circle,
        ColliderBoxComponent* box)
    {
        float closestX = std::clamp(circle->CircleCollider.x,
            box->ColliderBox.x, box->ColliderBox.x + box->ColliderBox.w);
        float closestY = std::clamp(circle->CircleCollider.y,
            box->ColliderBox.y, box->ColliderBox.y + box->ColliderBox.h);

        float dx = circle->CircleCollider.x - closestX;
        float dy = circle->CircleCollider.y - closestY;

        return (dx * dx + dy * dy) <
            (circle->CircleRadius * circle->CircleRadius);
    }
}