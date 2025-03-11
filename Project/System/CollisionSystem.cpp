#include <iostream>

#include "CollisionSystem.h"
#include "../Resources/ManagerGame.h"

// Component
#include "../Components/Colliders/ColliderComponent.h"
#include "../Components/PhysicsComponent.h"


void CollisionSystem::UpdateCollidion()
{
    // Обнаружение столкновений
    for (size_t i = 0; i < ManagerGame::objects.size(); ++i) {
        auto objA = ManagerGame::objects[i];
        auto colliderA = objA->GetComponent<ColliderComponent>();
        if (!colliderA) continue;

        for (size_t j = i + 1; j < ManagerGame::objects.size(); ++j) {
            auto objB = ManagerGame::objects[j];
            auto colliderB = objB->GetComponent<ColliderComponent>();
            if (!colliderB) continue;

            if (colliderA->CheckCollision(colliderB.get())) {
                // Обработка столкновения
                std::cout << "Collision between objects "
                    << i << " and " << j << std::endl;

                // Пример реакции на столкновение
                if (auto physicsA = objA->GetComponent<PhysicsComponent>()) {
                    physicsA->velocity.x *= -0.8f;
                    physicsA->velocity.y *= -0.8f;
                }
                if (auto physicsB = objB->GetComponent<PhysicsComponent>()) {
                    physicsB->velocity.x *= -0.8f;
                    physicsB->velocity.y *= -0.8f;
                }
            }
        }
    }
}
