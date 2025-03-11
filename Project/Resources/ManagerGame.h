#pragma once
#ifndef MANAGERGAME_H
#define MANAGERGAME_H

#include <vector>
#include <memory>
#include <SDL.h>

#include "../Core/GameObject.h"

class ManagerGame {
public:
    static SDL_Renderer* renderer; // Глобальный рендерер
    static std::vector<std::shared_ptr<GameObject>> objects;
};
#endif // MANAGERGAME_H