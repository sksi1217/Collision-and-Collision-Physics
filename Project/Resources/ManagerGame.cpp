#include "ManagerGame.h"

// Определение статического члена
SDL_Renderer* ManagerGame::renderer = nullptr;
std::vector<std::shared_ptr<GameObject>> ManagerGame::objects;