#include "GameObject.h"
#include <iostream>
#include <algorithm>

#include "../Resources/ManagerGame.h"

// Componet
#include "../Components/Colliders/ColliderComponent.h"
#include "../Components/Colliders/ColliderBoxComponent.h"
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"


// Обновление объекта
void GameObject::Update(float deltaTime) {
	auto transform = GetComponent<TransformComponent>();
	auto collider = GetComponent<ColliderComponent>();

	if (!transform || !collider) return;

	// Полиморфное обновление позиции коллайдера
	collider->UpdatePosition(transform->Position);
}

// Object rendering
void GameObject::Draw() {
	// Получаем компоненты
	auto transform = GetComponent<TransformComponent>();
	auto collider = GetComponent<ColliderComponent>();

	// Проверяем, что все необходимые компоненты существуют
	if (!transform || !collider) return;

	// Create a rectangle for floating point drawing
	SDL_FRect destRect = {
		(transform->Position.x) - transform->Origin.x * transform->Scale,
		(transform->Position.y) - transform->Origin.y * transform->Scale,
		static_cast<float>(transform->Scale),
		static_cast<float>(transform->Scale)
	};

	// Reflection flags
	SDL_RendererFlip flip = transform->FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	// Calculate the rotation point (in pixels after scaling)
	SDL_FPoint rotationCenter = {
		transform->Origin.x * transform->Scale,
		transform->Origin.y * transform->Scale
	};

	// Отрисовка Коллайдера
	if (collider) {
		SDL_SetRenderDrawColor(ManagerGame::renderer, 0, 255, 0, 255);

		// Правильное использование dynamic_cast с shared_ptr
		if (auto box = std::dynamic_pointer_cast<ColliderBoxComponent>(collider)) {
			SDL_FRect colliderRect = box->ColliderBox;
			SDL_RenderDrawRectF(ManagerGame::renderer, &colliderRect);
		}
		else if (auto circle = std::dynamic_pointer_cast<ColliderCircleComponent>(collider)) {
			RenderCircle(
				static_cast<int>(circle->CircleCollider.x),
				static_cast<int>(circle->CircleCollider.y),
				static_cast<int>(circle->CircleRadius)
			);
		}
	}
}

void GameObject::RenderCircle(int centerX, int centerY, int radius) {
	if (!ManagerGame::renderer) return;

	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;

	while (y >= 0) {
		// Отрисовка всех 4-х симметричных точек
		SDL_RenderDrawPoint(ManagerGame::renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(ManagerGame::renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(ManagerGame::renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(ManagerGame::renderer, centerX - x, centerY - y);

		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			x++;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			y--;
			delta += -2 * y + 1;
			continue;
		}
		x++;
		y--;
		delta += 2 * (x - y);
	}
}