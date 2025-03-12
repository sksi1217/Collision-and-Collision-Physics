#pragma once

#include "../Core/Component.h"
#include "../Core/Entities.h"

#include "../Resources/ManagerGame.h"
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/Colliders/ColliderBoxComponent.h"

class testObj : public Entities {
public:
    // Конструктор с параметрами
    testObj(const SDL_FPoint& startPosition);

    void Update(float deltaTime) override;

protected:
    void InitializeComponent(const SDL_FPoint& startPosition) override {
        Entities::InitializeComponent(startPosition);

        auto collider = std::make_shared<ColliderCircleComponent>();
        // collider->SetSize(SDL_FPoint{ 48, 48 });
        collider->CircleRadius = 50.0f;
        AddComponent(collider);
    }
private:
    // float ElapsedTime = 0;
    void ApplyFriction(PhysicsComponent* physics, float deltaTime);
    void UpdatePosition(PhysicsComponent* physics, float deltaTime);

};