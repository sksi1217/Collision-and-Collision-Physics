#pragma once
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <SDL.h>
#include "../Core/Component.h"

class TransformComponent : public Component {
public:
    SDL_FPoint Position = { 0.0f, 0.0f }; // Позиция
    SDL_FPoint prevPosition;    // Предыдущая позиция
    double Rotation = 0.0;              // Вращение
    float Scale = 1.0f;                 // Масштаб
    SDL_FPoint Origin = { 0.0f, 0.0f };   // центр вращения
    bool FlipHorizontal = false;        // Отражение по горизонтали

    void SetPosition(SDL_FPoint newPos) {
        prevPosition = Position;
        Position = newPos;
    }
};

#endif // TRANSFORM_COMPONENT_H