#pragma once

#include <SDL.h>

#include "../Core/Component.h"

// Component
#include "TransformComponent.h"
#include "Colliders/ColliderComponent.h"

// Components/PhysicsComponent.h
class PhysicsComponent : public Component {
public:
    SDL_FPoint Velocity{ 0.0f, 0.0f };
    float Mass = 1;
    float Restitution = 1; // Коэффициент упругости
    float Friction = 0.5f;  // Коэффициент трения (новое поле)
    bool IsStatic = false;  // Статический объект?
};