#include "Player.h"
#include <iostream>
#include "../Utils/MathUtils.h"

// Component
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/TransformComponent.h"

// Конструктор
Player::Player(const SDL_FPoint& startPosition) {
    InitializeComponent(startPosition);

    // Инициализация компонентов
    auto render = GetComponent<RenderComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();
    auto collider = std::dynamic_pointer_cast<ColliderCircleComponent>(GetComponent<ColliderComponent>());

    if (!transform || !physics || !collider) return;

    physics->Mass = 10;
    physics->Friction = 0.5;     // Коэффициент трения
    physics->Restitution = 1;  // Коэффициент упругости
    collider->CircleRadius = 40.0f; // Радиус коллизии

    transform->Position = startPosition;
}

void Player::Update(float deltaTime) {
    // Получаем состояние клавиш
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // Получаем компоненты
    auto physics = GetComponent<PhysicsComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto collider = GetComponent<ColliderComponent>();

    if (!physics || !transform) return;

    // Обнуляем скорость перед обработкой ввода
    SDL_FPoint velocity = { 0.0f, 0.0f };

    // Обработка движения
    // HandleMovement(keyboardState, velocity, deltaTime);

    // Применяем трение
    // ApplyFriction(physics.get(), deltaTime);

    // Обновляем позицию объекта
    UpdatePosition(physics.get(), deltaTime);


    // Обновление коллайдера
    if (collider) collider->UpdatePosition(transform->Position);
}

void Player::HandleMovement(const Uint8* keyboardState, SDL_FPoint& velocity, float deltaTime) {
    auto physics = GetComponent<PhysicsComponent>();
    if (!physics) return;

    // Скорость перемещения
    const float moveSpeed = 400;

    // Обработка нажатия клавиш
    if (keyboardState[SDL_SCANCODE_W]) velocity.y -= moveSpeed * deltaTime;
    if (keyboardState[SDL_SCANCODE_S]) velocity.y += moveSpeed * deltaTime;
    if (keyboardState[SDL_SCANCODE_A]) velocity.x -= moveSpeed * deltaTime;
    if (keyboardState[SDL_SCANCODE_D]) velocity.x += moveSpeed * deltaTime;

    // Применяем скорость к физике объекта
    physics->Velocity.x += velocity.x;
    physics->Velocity.y += velocity.y;

}

void Player::UpdatePosition(PhysicsComponent* physics, float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    if (!transform || !physics) return;

    // Применяем трение к скорости (уменьшаем влияние трения)
    physics->Velocity.x *= (1 - physics->Friction * deltaTime);
    physics->Velocity.y *= (1 - physics->Friction * deltaTime);



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
