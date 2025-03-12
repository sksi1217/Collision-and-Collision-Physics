#pragma once
#include <SDL.h>
#include <typeinfo>

#include "../../Core/Component.h"

class ColliderComponent : public Component {
public:
    const std::type_info& GetStoredType() const override {
        return typeid(ColliderComponent);
    }

    virtual ~ColliderComponent() = default;

    void SetOffset(const SDL_FPoint& newOffset) {
        offset = newOffset;
        UpdatePosition(currentPosition);
    }

    virtual void UpdatePosition(const SDL_FPoint& position) = 0;

protected:
    SDL_FPoint offset{ 0.0f, 0.0f };
    SDL_FPoint currentPosition{ 0.0f, 0.0f };
};