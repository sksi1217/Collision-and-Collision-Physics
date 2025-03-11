#include <algorithm>

#include "ColliderCircleComponent.h"
#include "ColliderBoxComponent.h"

// ColliderCircleComponent.cpp
bool ColliderCircleComponent::CheckCollision(ColliderComponent* other) {
    if (auto circle = dynamic_cast<ColliderCircleComponent*>(other)) {
        return CheckCircleCircle(this, circle);
    }
    if (auto box = dynamic_cast<ColliderBoxComponent*>(other)) {
        // return CheckCircleBox(this, box);
        return 0;
    }
    return false;
}

bool ColliderCircleComponent::CheckCircleCircle(ColliderCircleComponent* a, ColliderCircleComponent* b) {
    float dx = a->CircleCollider.x - b->CircleCollider.x;
    float dy = a->CircleCollider.y - b->CircleCollider.y;
    float distanceSq = dx * dx + dy * dy;
    float radiusSum = a->CircleRadius + b->CircleRadius;
    return distanceSq < radiusSum * radiusSum;
}