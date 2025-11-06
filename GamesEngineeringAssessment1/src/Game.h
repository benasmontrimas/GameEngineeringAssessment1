#pragma once

#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Sprite.h"
#include "FontMap.h"
#include "Level.h"

static constexpr int MAX_ENEMIES = 1000;

// I will want to add animations at some point
enum GameImages : unsigned char {
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

	PlayerDeath1,
	PlayerDeath2,
	PlayerDeath3,
	PlayerDeath4,
	PlayerDeath5,
	PlayerDeath6,
	PlayerDeath7,
	PlayerDeath8,
	PlayerDeath9,
	PlayerDeath10,
	PlayerDeath11,
	PlayerDeath12,
	PlayerDeath13,
	PlayerDeath14,
	PlayerDeath15,
	PlayerDeath16,
	PlayerDeath17,
	PlayerDeath18,

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

	SwordsmanDeath1,
	SwordsmanDeath2,
	SwordsmanDeath3,
	SwordsmanDeath4,
	SwordsmanDeath5,
	SwordsmanDeath6,
	SwordsmanDeath7,
	SwordsmanDeath8,
	SwordsmanDeath9,
	SwordsmanDeath10,

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

	ArcherAttack1,
	ArcherAttack2,
	ArcherAttack3,
	ArcherAttack4,
	ArcherAttack5,
	ArcherAttack6,
	ArcherAttack7,
	ArcherAttack8,

	ArcherDeath1,
	ArcherDeath2,
	ArcherDeath3,
	ArcherDeath4,
	ArcherDeath5,
	ArcherDeath6,
	ArcherDeath7,
	ArcherDeath8,
	ArcherDeath9,
	ArcherDeath10,
	ArcherDeath11,
	ArcherDeath12,

	ArcherArrow1,
	ArcherArrow2,
	ArcherArrow3,
	ArcherArrow4,

	TurretIdle1,
	TurretIdle2,
	TurretIdle3,

	TurretAttack1,
	TurretAttack2,
	TurretAttack3,
	TurretAttack4,
	TurretAttack5,

	TurretDeath1,
	TurretDeath2,
	TurretDeath3,
	TurretDeath4,
	TurretDeath5,
	TurretDeath6,
	TurretDeath7,
	TurretDeath8,

	BomberWalk1,
	BomberWalk2,
	BomberWalk3,
	BomberWalk4,
	BomberWalk5,
	BomberWalk6,
	BomberWalk7,
	BomberWalk8,
	BomberWalk9,
	BomberWalk10,

	BomberAttack1,
	BomberAttack2,
	BomberAttack3,
	BomberAttack4,
	BomberAttack5,
	BomberAttack6,

	BomberDeath1,
	BomberDeath2,
	BomberDeath3,
	BomberDeath4,
	BomberDeath5,
	BomberDeath6,
	BomberDeath7,
	BomberDeath8,
	BomberDeath9,
	BomberDeath10,
	BomberDeath11,
	BomberDeath12,

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