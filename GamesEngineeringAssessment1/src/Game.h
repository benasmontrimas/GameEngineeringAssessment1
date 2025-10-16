#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"
#include "Camera.h"

class Game {
public:
	Game() = default;

	void Init();
	void Render();
	void Run();

private:
	GamesEngineeringBase::Window window{};
	Player player{};
	Camera camera{};
	bool running = false;
};