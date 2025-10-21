#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Map.h"
#include "HUD.h"
#include "Sprite.h"
#include "FontMap.h"

static const int MAX_ENEMIES = 1000;

// I will want to add animations at some point
enum GameImages {
	PlayerBase,
	PlayerWalk2,
	PlayerWalk3,
	ZombieBase,
	ArcherBase,
	TankBase,

	ItemBorder,

	AOEWeapon,

	Tile0,
	Tile1,
	Tile2,
	Tile3,
	Tile4,
	Tile5,
	Tile6,
	Tile7,
	Tile8,
	Tile9,
	Tile10,

	Font32pt,
	Font16pt,

	GAME_IMAGE_COUNT
};

class Game {
public:
	Game() = default;
	~Game() = default;

	Game(Game& g) = delete;
	Game(Game&& g) = delete;

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
	TileMap level_map{};
	Player player{};
	Enemy enemies[MAX_ENEMIES];
	Camera camera{};
	HUD hud{};

	FontMap font32;
	FontMap font16;

	// Other
	int window_width;
	int window_height;
	int* depth_buffer;
	float delta_time;
	bool running = false;
};