#pragma once

#include <SDL.h>
#include "../Core/Component.h"

class MovementComponent : public Component {
public:
    float m_movementSpeed = 10.0f;                 // Скорость
    float m_movementSpeedModifier = 1.0f; // Модификатор скорости (для эффектов)
};