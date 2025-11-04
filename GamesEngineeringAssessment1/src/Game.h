#pragma once

#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Sprite.h"
#include "FontMap.h"
#include "Level.h"

static constexpr int MAX_ENEMIES = 1000;

// I will want to add animations at some point
enum GameImages : unsigned char {
	MainMenuBG,
	MainMenuHeader,
	ButtonBG,
	PopupMenuBG,

	PlayerIdle1,
	PlayerIdle2,
	PlayerIdle3,
	PlayerIdle4,
	PlayerIdle5,
	PlayerIdle6,
	PlayerIdle7,

	PlayerWalk1,
	PlayerWalk2,
	PlayerWalk3,
	PlayerWalk4,
	PlayerWalk5,
	PlayerWalk6,
	PlayerWalk7,
	PlayerWalk8,
	PlayerWalk9,

	PlayerBase,

	SwordsmanWalk1,
	SwordsmanWalk2,
	SwordsmanWalk3,
	SwordsmanWalk4,
	SwordsmanWalk5,
	SwordsmanWalk6,
	SwordsmanWalk7,
	SwordsmanWalk8,
	SwordsmanWalk9,
	SwordsmanWalk10,

	SwordsmanAttack1,
	SwordsmanAttack2,
	SwordsmanAttack3,
	SwordsmanAttack4,
	SwordsmanAttack5,
	SwordsmanAttack6,
	SwordsmanAttack7,
	SwordsmanAttack8,
	SwordsmanAttack9,
	SwordsmanAttack10,
	SwordsmanAttack11,
	SwordsmanAttack12,
	SwordsmanAttack13,
	SwordsmanAttack14,
	SwordsmanAttack15,

	ArcherWalk1,
	ArcherWalk2,
	ArcherWalk3,
	ArcherWalk4,
	ArcherWalk5,
	ArcherWalk6,
	ArcherWalk7,
	ArcherWalk8,
	ArcherWalk9,
	ArcherWalk10,

	TankBase,

	ItemBorder,

	Weapon1, // TODO: Name these at some point

	SwordThrow1,
	SwordThrow2,
	SwordThrow3,
	SwordThrow4,
	SwordThrow5,
	SwordThrow6,
	SwordThrow7,
	SwordThrow8,

	Water1,
	Water2,
	Water3,
	Water4,
	Water5,
	Water6,
	Water7,
	Water8,

	Grass1,

	Path1,

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

	Font32Pt,
	Font16Pt,

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

	void DrawSprite(const Sprite& sprite, const Vec2& position);
	void DrawSprite(const Sprite& sprite, const Vec2& position, float rotation, const Vec2& around);
	void DrawSpriteScreenSpace(const Sprite& sprite, const Vec2& position);

	bool SetNextLevel(Level* level);

	Level* GetLevel() const;

private:
	void Render();
	void Update();
	void LoadAssets();
	void SwitchLevel();

private:
	Level* active_level_;
	Level* next_level_;

public:
	// Systems
	GamesEngineeringBase::Window window{};
	GamesEngineeringBase::Image images[GAME_IMAGE_COUNT];
	GamesEngineeringBase::Timer timer{};

	Camera camera{};

	FontMap font32{};
	FontMap font16{};

	// Other
	unsigned int window_width{};
	unsigned int window_height{};
	int* depth_buffer{};

	// Need to separate out game time and real time for pausing.
	float delta_time{};
	float game_time_multiplier = 1.0f;
	float game_time{};

	int fps{0};

	bool running = false;
};