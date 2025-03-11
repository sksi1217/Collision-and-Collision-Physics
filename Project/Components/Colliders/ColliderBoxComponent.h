#pragma once

#include "ColliderComponent.h"
#include "ColliderCircleComponent.h"

class ColliderBoxComponent : public ColliderComponent {
public:
    SDL_FRect ColliderBox{ 0.0f, 0.0f, 8.0f, 8.0f };

    void UpdatePosition(const SDL_FPoint& position) override {
        ColliderBox.x = position.x + offset.x;
        ColliderBox.y = position.y + offset.y;
    }

    void SetSize(const SDL_FPoint& newSize) {
        ColliderBox.w = newSize.x;
        ColliderBox.h = newSize.y;
    }

    bool CheckCollision(ColliderComponent* other) override;

private:
    static bool CheckBoxBox(ColliderBoxComponent* a, ColliderBoxComponent* b);
    static bool CheckBoxCircle(ColliderBoxComponent* box, ColliderCircleComponent* circle);
};