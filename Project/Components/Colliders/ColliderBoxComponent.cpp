#include <algorithm>

#include "ColliderBoxComponent.h"

bool ColliderBoxComponent::CheckCollision(ColliderComponent* other) {
    if (auto box = dynamic_cast<ColliderBoxComponent*>(other)) {
        return CheckBoxBox(this, box);
    }
    if (auto circle = dynamic_cast<ColliderCircleComponent*>(other)) {
        return CheckBoxCircle(this, circle);
    }
    return false;
}

bool ColliderBoxComponent::CheckBoxBox(ColliderBoxComponent* a, ColliderBoxComponent* b) {
    return !(a->ColliderBox.x + a->ColliderBox.w < b->ColliderBox.x ||
        a->ColliderBox.x > b->ColliderBox.x + b->ColliderBox.w ||
        a->ColliderBox.y + a->ColliderBox.h < b->ColliderBox.y ||
        a->ColliderBox.y > b->ColliderBox.y + b->ColliderBox.h);
}

bool ColliderBoxComponent::CheckBoxCircle(ColliderBoxComponent* box, ColliderCircleComponent* circle) {
    float closestX = std::clamp(circle->CircleCollider.x,
        box->ColliderBox.x, box->ColliderBox.x + box->ColliderBox.w);
    float closestY = std::clamp(circle->CircleCollider.y,
        box->ColliderBox.y, box->ColliderBox.y + box->ColliderBox.h);

    float distanceX = circle->CircleCollider.x - closestX;
    float distanceY = circle->CircleCollider.y - closestY;

    return (distanceX * distanceX + distanceY * distanceY) <
        (circle->CircleRadius * circle->CircleRadius);
}
