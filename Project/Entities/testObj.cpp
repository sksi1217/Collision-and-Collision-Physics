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

    physics->Mass = 10;           // Масса объекта
    physics->Friction = 1;        // Коэффициент трения (не используется)
    physics->Restitution = 1;     // Коэффициент упругости
    collider->CircleRadius = 40.0f; // Радиус коллизии

    transform->Position = startPosition;
}

void testObj::Update(float deltaTime) {
    auto physics = GetComponent<PhysicsComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto collider = GetComponent<ColliderComponent>();

    if (!physics || !transform) return;

    // Обработка входных данных
    HandleMovement(physics, deltaTime);

    // Ограничение максимальной скорости
    LimitSpeed(physics);

    // Обновляем позицию
    transform->Position.x += physics->Velocity.x * deltaTime;
    transform->Position.y += physics->Velocity.y * deltaTime;

    std::cout << physics->Velocity.x << "/" << physics->Velocity.y << std::endl;

    // Обновляем коллайдер
    if (collider) collider->UpdatePosition(transform->Position);
}

// Метод для обработки пользовательского ввода
void testObj::HandleMovement(std::shared_ptr<PhysicsComponent> physics, float deltaTime) {
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // Сбрасываем скорость перед обработкой ввода
    SDL_FPoint direction = { 0.0f, 0.0f };

    // Заполняем направление движения на основе нажатых клавиш
    if (keyboardState[SDL_SCANCODE_W]) direction.y -= 1.0f; // Вверх
    if (keyboardState[SDL_SCANCODE_S]) direction.y += 1.0f; // Вниз
    if (keyboardState[SDL_SCANCODE_A]) direction.x -= 1.0f; // Влево
    if (keyboardState[SDL_SCANCODE_D]) direction.x += 1.0f; // Вправо

    // Нормализуем направление, если оно не равно нулю
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
    }

    // Устанавливаем скорость на основе направления
    physics->Velocity.x = direction.x;
    physics->Velocity.y = direction.y;
}

// Метод для ограничения максимальной скорости
void testObj::LimitSpeed(std::shared_ptr<PhysicsComponent> physics) {
    float currentSpeed = std::sqrt(physics->Velocity.x * physics->Velocity.x + physics->Velocity.y * physics->Velocity.y);

    if (currentSpeed > 0.0f) {
        // Нормализуем вектор скорости и умножаем на максимальную скорость
        float scale = maxSpeed / currentSpeed;
        physics->Velocity.x *= scale;
        physics->Velocity.y *= scale;
    }
}