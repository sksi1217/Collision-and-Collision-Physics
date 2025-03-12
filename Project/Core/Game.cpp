// Game.cpp
#include <iostream>
#include "Game.h"

#include "../Resources/ManagerGame.h"

// System
#include "../System/CollisionSystem.h"

// Component
#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/Colliders/ColliderBoxComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Entities/Player.h"
#include "../Entities/testObj.h"


CollisionSystem сollisionSystem;

std::shared_ptr<GameObject> followObject = nullptr;

// Designer
Game::Game(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		SDL_Quit();
		isRunning = false;
		return;
	}


	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}

	ManagerGame::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!ManagerGame::renderer) {
		std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}
}

// Destructor
Game::~Game() {
	std::cout << "Destroying renderer and window..." << std::endl;
	SDL_DestroyRenderer(ManagerGame::renderer);
	SDL_DestroyWindow(window);

	std::cout << "Quitting SDL_image and SDL..." << std::endl;
	IMG_Quit();
	SDL_Quit();
}


// Initialization
void Game::Initialize() {
	std::cout << "Game initialized!" << std::endl;
}

// Resource Loading
void Game::LoadContent() {
	std::cout << "Loading content..." << std::endl;

	// Create test obj

	std::shared_ptr<GameObject> player = std::make_shared<Player>(SDL_FPoint{ 100, 100 });
	ManagerGame::objects.push_back(player);

	// ---

	std::shared_ptr<GameObject> obj = std::make_shared<testObj>(SDL_FPoint{ 200, 100 });
	ManagerGame::objects.push_back(obj);

	/*
	auto obj2 = std::make_shared<GameObject>();

	auto transform = std::make_shared<TransformComponent>();
	transform->Position = { 200, 100 };
	obj2->AddComponent(transform);

	auto renderer = std::make_shared<RenderComponent>();
	obj2->AddComponent(renderer);

	auto physics = std::make_shared<PhysicsComponent>();
	obj2->AddComponent(physics);

	auto collider = std::make_shared<ColliderCircleComponent>();
	// collider->SetSize(SDL_FPoint{ 64, 64 });
	collider->CircleRadius = 60.0f;
	obj2->AddComponent(collider);

	ManagerGame::objects.push_back(obj2);
	*/
	followObject = player;
}

// Logic Update
void Game::Update(float deltaTime)
{
	

	for (size_t i = 0; i < ManagerGame::objects.size(); ) {
		auto& obj = ManagerGame::objects[i];
		// Обновляем объект, если он активен
		obj->Update(deltaTime);
		++i; // Переходим к следующему объекту
	}

	UpdateBoundaries(800, 600);

	сollisionSystem.UpdateCollisions();
	

}

// Frame rendering
void Game::Draw() {
	SDL_SetRenderDrawColor(ManagerGame::renderer, 50, 50, 50, 255);
	SDL_RenderClear(ManagerGame::renderer);

	// Drawing of all objects
	for (auto& obj : ManagerGame::objects) {
		obj->Draw();
	}

	SDL_RenderPresent(ManagerGame::renderer);
}

// Выгрузка ресурсов
void Game::UnloadContent() {
	std::cout << "Unloading content..." << std::endl;
}

void Game::HandleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
			/*
		case SDL_MOUSEMOTION: {
			if (followObject) {
				int x, y;
				SDL_GetMouseState(&x, &y);

				auto transform = followObject->GetComponent<TransformComponent>();
				auto physics = followObject->GetComponent<PhysicsComponent>();

				if (transform && physics) {
					// Вычисляем вектор направления к курсору
					float dx = x - transform->Position.x;
					float dy = y - transform->Position.y;

					// Нормализуем вектор направления
					float distance = std::sqrt(dx * dx + dy * dy);
					if (distance > 0) {
						dx /= distance;
						dy /= distance;
					}

					// Задаем максимальную скорость движения к курсору
					const float maxSpeed = 300.0f;

					// Устанавливаем скорость объекта в направлении курсора
					physics->Velocity.x = dx * maxSpeed;
					physics->Velocity.y = dy * maxSpeed;

					// Если объект уже близко к курсору, останавливаем его
					if (distance < 5.0f) { // Пороговое значение для остановки
						physics->Velocity.x = 0.0f;
						physics->Velocity.y = 0.0f;
					}
				}

				// Обновляем коллайдер
				if (auto collider = followObject->GetComponent<ColliderComponent>()) {
					collider->UpdatePosition(transform->Position);
				}
			}
			break;
		}
		*/
		}
	}
}

void Game::Run() {
	Initialize();
	LoadContent();

	const int TARGET_FPS = 60;
	const Uint32 TARGET_FRAME_TIME = 1000 / TARGET_FPS; // Time of one frame in milliseconds

	Uint32 lastTime = SDL_GetTicks(); // Time of previous frame

	while (isRunning) {
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f; // Time difference between frames
		lastTime = currentTime;

		// Logic Update
		Update(deltaTime);

		Draw();

		HandleEvents();

		// Restriction FPS
		Uint32 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < TARGET_FRAME_TIME) {
			SDL_Delay(TARGET_FRAME_TIME - frameTime);
		}
	}

	UnloadContent();
}

void Game::UpdateBoundaries(int screenWidth, int screenHeight) {
	auto& objects = ManagerGame::objects;

	for (auto& obj : objects) {
		auto transform = obj->GetComponent<TransformComponent>();
		auto physics = obj->GetComponent<PhysicsComponent>();
		auto collider = obj->GetComponent<ColliderComponent>();

		auto colliderA = std::dynamic_pointer_cast<ColliderCircleComponent>(collider);

		if (!transform || !physics || !colliderA) continue;

		// Проверяем границы экрана
		if (transform->Position.x - colliderA->CircleRadius < 0) { // Левая граница
			transform->Position.x = colliderA->CircleRadius;
			physics->Velocity.x = std::abs(physics->Velocity.x) * physics->Restitution; // Отскок
		}
		if (transform->Position.x + colliderA->CircleRadius > screenWidth) { // Правая граница
			transform->Position.x = screenWidth - colliderA->CircleRadius;
			physics->Velocity.x = -std::abs(physics->Velocity.x) * physics->Restitution; // Отскок
		}
		if (transform->Position.y - colliderA->CircleRadius < 0) { // Верхняя граница
			transform->Position.y = colliderA->CircleRadius;
			physics->Velocity.y = std::abs(physics->Velocity.y) * physics->Restitution; // Отскок
		}
		if (transform->Position.y + colliderA->CircleRadius > screenHeight) { // Нижняя граница
			transform->Position.y = screenHeight - colliderA->CircleRadius;
			physics->Velocity.y = -std::abs(physics->Velocity.y) * physics->Restitution; // Отскок
		}
	}
}