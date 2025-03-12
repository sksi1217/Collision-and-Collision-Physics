#pragma once

#include "GameObject.h"

// Component
#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/MovementComponent.h"

class Entities : public GameObject
{
public:
	virtual void InitializeComponent(const SDL_FPoint& startPosition)
	{
		auto transform = std::make_shared<TransformComponent>();
		AddComponent(transform);

		auto physics = std::make_shared<PhysicsComponent>();
		AddComponent(physics);

		auto render = std::make_shared<RenderComponent>();
		AddComponent(render);

		auto movement = std::make_shared<MovementComponent>();
		AddComponent(movement);
	}
};