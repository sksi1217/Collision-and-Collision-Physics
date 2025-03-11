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

	auto obj1 = std::make_shared<GameObject>();

	auto transform = std::make_shared<TransformComponent>();
	transform->Position = { 100, 100 };
	obj1->AddComponent(transform);

	auto renderer = std::make_shared<RenderComponent>();
	obj1->AddComponent(renderer);

	auto collider = std::make_shared<ColliderCircleComponent>();
	collider->CircleRadius = 40.0f;
	obj1->AddComponent(collider);

	ManagerGame::objects.push_back(obj1);

	// ---

	auto obj2 = std::make_shared<GameObject>();

	auto transform1 = std::make_shared<TransformComponent>();
	transform1->Position = { 100, 100 };
	obj2->AddComponent(transform1);

	auto renderer1 = std::make_shared<RenderComponent>();
	obj2->AddComponent(renderer1);

	auto collider1 = std::make_shared<ColliderCircleComponent>();
	collider1->CircleRadius = 30.0f;
	obj2->AddComponent(collider1);

	ManagerGame::objects.push_back(obj2);

	followObject = obj1;
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

	сollisionSystem.UpdateCollidion();
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

// Game.cpp
// В Game.cpp
void Game::HandleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_MOUSEMOTION: {
			if (followObject) {
				int x, y;
				SDL_GetMouseState(&x, &y);

				auto transform = followObject->GetComponent<TransformComponent>();
				if (transform) {
					transform->Position.x += (x - transform->Position.x);
					transform->Position.y += (y - transform->Position.y);

					// Обновляем коллайдер
					if (auto collider = followObject->GetComponent<ColliderComponent>()) {
						collider->UpdatePosition(transform->Position);
					}
				}
			}
			break;
		}
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