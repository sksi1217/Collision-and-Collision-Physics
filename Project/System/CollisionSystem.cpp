#include <algorithm>
#include <iostream>

// System
#include "CollisionSystem.h"
#include "CollisionChecks.h"

// Component
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/Colliders/ColliderBoxComponent.h"
#include "../Components/PhysicsComponent.h"

void CollisionSystem::UpdateCollisions() {
	auto& objects = ManagerGame::objects;
	for (size_t i = 0; i < objects.size(); ++i) {
		auto objA = objects[i];
		auto colliderA = objA->GetComponent<ColliderComponent>();
		if (!colliderA) continue;

		for (size_t j = i + 1; j < objects.size(); ++j) {
			auto objB = objects[j];
			auto colliderB = objB->GetComponent<ColliderComponent>();
			if (!colliderB) continue;

			if (CheckCollision(colliderA.get(), colliderB.get())) {
				ResolveCollision(objA.get(), objB.get());
			}
		}
	}
}

bool CollisionSystem::CheckCollision(ColliderComponent* a, ColliderComponent* b) {
	if (auto circleA = dynamic_cast<ColliderCircleComponent*>(a)) {
		if (auto circleB = dynamic_cast<ColliderCircleComponent*>(b)) {
			// std::cout << "CheckCircleCircle" << std::endl;
			return CollisionChecks::CheckCircleCircle(circleA, circleB);
		}
		if (auto boxB = dynamic_cast<ColliderBoxComponent*>(b)) {
			// std::cout << "CheckCircleBox" << std::endl;
			return CollisionChecks::CheckCircleBox(circleA, boxB);
		}
	}
	if (auto boxA = dynamic_cast<ColliderBoxComponent*>(a)) {
		if (auto boxB = dynamic_cast<ColliderBoxComponent*>(b)) {
			std::cout << "CheckBoxBox" << std::endl;
			return CollisionChecks::CheckBoxBox(boxA, boxB);
		}
		if (auto circleB = dynamic_cast<ColliderCircleComponent*>(b)) {
			std::cout << "CheckCircleBox" << std::endl;
			return CollisionChecks::CheckCircleBox(circleB, boxA);
		}
	}
	return false;
}

float CollisionSystem::GetInverseMass(float mass) {
	return (mass == 0) ? 0 : 1 / mass;
}

void CollisionSystem::ResolveCollision(GameObject* a, GameObject* b) {
	// Получаем компоненты объектов
	auto transformA = a->GetComponent<TransformComponent>();
	auto colliderAA = a->GetComponent<ColliderComponent>();
	auto physicsA = a->GetComponent<PhysicsComponent>();

	auto transformB = b->GetComponent<TransformComponent>();
	auto colliderBB = b->GetComponent<ColliderComponent>();
	auto physicsB = b->GetComponent<PhysicsComponent>();

	auto colliderA = std::dynamic_pointer_cast<ColliderCircleComponent>(colliderAA);
	auto colliderB = std::dynamic_pointer_cast<ColliderCircleComponent>(colliderBB);

	if (!transformA || !colliderA || !physicsA || !transformB || !colliderB || !physicsB) return;

	// Вычисляем относительную скорость
	SDL_FPoint rv = { physicsB->Velocity.x - physicsA->Velocity.x,
					  physicsB->Velocity.y - physicsA->Velocity.y };

	// Вычисляем нормаль столкновения (вектор между центрами объектов)
	SDL_FPoint normal = { colliderB->CircleCollider.x - colliderA->CircleCollider.x,
						  colliderB->CircleCollider.y - colliderA->CircleCollider.y };
	float distance = std::sqrt(normal.x * normal.x + normal.y * normal.y);
	if (distance == 0) return; // Центры совпадают, ничего не делаем

	// Нормализуем нормаль
	normal.x /= distance;
	normal.y /= distance;

	// Находим проекцию относительной скорости на нормаль столкновения
	float velAlongNormal = rv.x * normal.x + rv.y * normal.y;

	// Проверяем, нужно ли разрешать столкновение
	if (velAlongNormal > 0) return; // Если объекты уже расходятся, ничего не делаем

	// Вычисляем коэффициент упругости
	float e = std::min(physicsA->Restitution, physicsB->Restitution);

	// Вычисляем величину импульса (j)
	float j = -(1 + e) * velAlongNormal;
	j /= (GetInverseMass(physicsA->Mass) + GetInverseMass(physicsB->Mass));

	// Применяем нормальный импульс к скоростям объектов
	SDL_FPoint impulse = { j * normal.x, j * normal.y };

	physicsA->Velocity.x -= GetInverseMass(physicsA->Mass) * impulse.x;
	physicsA->Velocity.y -= GetInverseMass(physicsA->Mass) * impulse.y;

	physicsB->Velocity.x += GetInverseMass(physicsB->Mass) * impulse.x;
	physicsB->Velocity.y += GetInverseMass(physicsB->Mass) * impulse.y;

	// Вычисляем тангенциальную скорость
	SDL_FPoint tangent = { rv.x - velAlongNormal * normal.x,
						   rv.y - velAlongNormal * normal.y };

	// Нормализуем тангенциальный вектор
	float tangentLength = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);
	if (tangentLength > 0) {
		tangent.x /= tangentLength;
		tangent.y /= tangentLength;
	}
	else {
		tangent = { 0.0f, 0.0f }; // Если тангенциальная скорость нулевая, обнуляем вектор
	}

	// Вычисляем коэффициент трения
	float frictionCoefficient = std::min(physicsA->Friction, physicsB->Friction);

	// Вычисляем импульс трения
	float jt = -frictionCoefficient * j;

	// Применяем импульс трения
	SDL_FPoint impulseFriction = { jt * tangent.x, jt * tangent.y };

	physicsA->Velocity.x += GetInverseMass(physicsA->Mass) * impulseFriction.x;
	physicsA->Velocity.y += GetInverseMass(physicsA->Mass) * impulseFriction.y;

	physicsB->Velocity.x -= GetInverseMass(physicsB->Mass) * impulseFriction.x;
	physicsB->Velocity.y -= GetInverseMass(physicsB->Mass) * impulseFriction.y;

	// Обновляем позиции объектов, если они пересекаются
	float overlap = distance - (colliderA->CircleRadius + colliderB->CircleRadius);

	if (overlap < 0) {
		// Коррекция позиции
		PositionalCorrection(a, b, -overlap, normal);
	}
}

void CollisionSystem::PositionalCorrection(GameObject* a, GameObject* b, float penetrationDepth, SDL_FPoint normal) {
	const float slop = 0.1f;   // Пороговое значение для игнорирования небольших проникновений

	auto physicsA = a->GetComponent<PhysicsComponent>();
	auto physicsB = b->GetComponent<PhysicsComponent>();

	if (!physicsA || !physicsB) return;

	// Вычисляем обратные массы
	float invMassA = GetInverseMass(physicsA->Mass);
	float invMassB = GetInverseMass(physicsB->Mass);

	// Вычисляем коррекцию
	float correctionMagnitude = std::max(penetrationDepth - slop, 0.0f) /
		(invMassA + invMassB);

	SDL_FPoint correction = { correctionMagnitude * normal.x, correctionMagnitude * normal.y };

	// Применяем коррекцию к позициям объектов
	auto transformA = a->GetComponent<TransformComponent>();
	transformA->Position.x -= invMassA * correction.x;
	transformA->Position.y -= invMassA * correction.y;

	auto transformB = b->GetComponent<TransformComponent>();
	transformB->Position.x += invMassB * correction.x;
	transformB->Position.y += invMassB * correction.y;
}
