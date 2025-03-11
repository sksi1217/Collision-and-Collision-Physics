#pragma once

#include <SDL.h>

#include "../Core/Component.h"

// Component
#include "TransformComponent.h"
#include "Colliders/ColliderComponent.h"

class PhysicsComponent : public Component {
public:
    // Основные физические параметры
    SDL_FPoint velocity{ 0.0f, 0.0f };   // Скорость (пиксели/сек)
    SDL_FPoint acceleration{ 0.0f, 0.0f }; // Ускорение
    float mass = 1.0f;                // Масса объекта
    bool isStatic = false;            // Статический объект?
    float friction = 0.1f;            // Коэффициент трения
};