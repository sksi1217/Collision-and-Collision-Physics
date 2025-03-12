#pragma once

#include <memory>

// Component
#include "../Components/Colliders/ColliderComponent.h"
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/Colliders/ColliderBoxComponent.h"
#include "../Components/PhysicsComponent.h"

#include "../Resources/ManagerGame.h"


class CollisionSystem {
public:
    void UpdateCollisions();
    float GetInverseMass(float mass);


private:
    bool CheckCollision(ColliderComponent* a, ColliderComponent* b);
    void ResolveCollision(GameObject* a, GameObject* b);
    void PositionalCorrection(GameObject* a, GameObject* b, float penetrationDepth, SDL_FPoint normal);
};