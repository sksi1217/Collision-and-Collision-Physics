﻿#pragma once

#include "ColliderComponent.h"

class ColliderCircleComponent : public ColliderComponent {
public:
    SDL_FPoint CircleCollider { 0.0f, 0.0f };
    float CircleRadius { 2.0f };

    void UpdatePosition(const SDL_FPoint& position) override {
        CircleCollider.x = position.x + offset.x;
        CircleCollider.y = position.y + offset.y;
    }
};