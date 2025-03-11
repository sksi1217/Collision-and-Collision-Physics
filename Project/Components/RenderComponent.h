#pragma once

#include <SDL.h>
#include "../Core/Component.h"

class RenderComponent : public Component {
public:
    // SDL_Texture* Texture = nullptr;     // Текстура
    SDL_Color TintColor = { 255, 255, 255, 255 }; // Цвет фильтра
    bool IsVisible = true;              // Видимость
};