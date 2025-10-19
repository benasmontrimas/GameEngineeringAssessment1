#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Sprite.h"

static const int MAX_ENEMIES = 1000;

// I will want to add animations at some point
enum GameImages {
	PlayerBase,
	ZombieBase,
	ArcherBase,
	TankBase,

	GAME_IMAGE_COUNT
};

class Game {
public:
	Game() = default;

	void Init();
	void Run();

	void DrawImage(const Sprite& image, const Vec2& position);

private:
	void Render();
	void Update();
	void LoadAssets();

public:
	// Systems
	GamesEngineeringBase::Window window{};
	GamesEngineeringBase::Image images[GAME_IMAGE_COUNT];
	GamesEngineeringBase::Timer timer{};

	// Entities
	Player player{};
	Enemy enemies[MAX_ENEMIES];
	Camera camera{};

	// Other
	int window_width;
	int window_height;
	int* depth_buffer;
	float delta_time;
	bool running = false;
};