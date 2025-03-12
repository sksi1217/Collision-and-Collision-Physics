#include "testObj.h"
#include <iostream>
#include "../Utils/MathUtils.h"

// Component
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/TransformComponent.h"

// Конструктор
testObj::testObj(const SDL_FPoint& startPosition) {
    InitializeComponent(startPosition);

    // Инициализация компонентов
    auto render = GetComponent<RenderComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();
    auto collider = std::dynamic_pointer_cast<ColliderCircleComponent>(GetComponent<ColliderComponent>());

    if (!transform || !physics || !collider) return;

    physics->Mass = 999;           // Масса объекта
    physics->Friction = 0.5;     // Коэффициент трения
    physics->Restitution = 1;  // Коэффициент упругости
    collider->CircleRadius = 40.0f; // Радиус коллизии

    transform->Position = startPosition;
}

void testObj::Update(float deltaTime) {
    // Получаем компоненты
    auto physics = GetComponent<PhysicsComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto collider = GetComponent<ColliderComponent>();

    if (!physics || !transform) return;

    // Обновляем позицию объекта
    UpdatePosition(physics.get(), deltaTime);

    // Обновление коллайдера
    if (collider) collider->UpdatePosition(transform->Position);
}

void testObj::UpdatePosition(PhysicsComponent* physics, float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    if (!transform || !physics) return;

    // Применяем трение к скорости (уменьшаем влияние трения)
    physics->Velocity.x *= (1 - physics->Friction * deltaTime);
    physics->Velocity.y *= (1 - physics->Friction * deltaTime);

    std::cout << physics->Velocity.x << "/" << physics->Velocity.y << std::endl;


    // Порог для обнуления скорости (очень маленькое значение)
    const float velocityThreshold = 0.1f;

    // Если скорость очень маленькая, обнуляем её
    if (std::abs(physics->Velocity.x) <= velocityThreshold) {
        physics->Velocity.x = 0.0f;
    }
    if (std::abs(physics->Velocity.y) <= velocityThreshold) {
        physics->Velocity.y = 0.0f;
    }

    // Обновляем позицию на основе скорости
    transform->Position.x += physics->Velocity.x * deltaTime;
    transform->Position.y += physics->Velocity.y * deltaTime;
}