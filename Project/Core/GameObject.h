#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <unordered_map>
#include <string>
#include <memory>
#include <SDL.h>

#include "Component.h"

class GameObject {
public:
    std::unordered_map<std::string, std::shared_ptr<Component>> components;

    template <typename T>
    void AddComponent(std::shared_ptr<T> component) {
        // Используем GetStoredType() вместо typeid(T)
        components[component->GetStoredType().name()] = component;
    }

    template <typename T>
    std::shared_ptr<T> GetComponent() {
        return std::static_pointer_cast<T>(components[typeid(T).name()]);
    }

    virtual void Update(float deltaTime);
    virtual void Draw();

private:
    void RenderCircle(int centerX, int centerY, int radius);
};

#endif // GAME_OBJECT_H