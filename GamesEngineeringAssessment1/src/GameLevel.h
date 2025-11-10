#pragma once

#include <random>

#include "Enemy.h"
#include "HUD.h"
#include "Level.h"
#include "TileMap.h"
#include "PauseMenu.h"
#include "EndGameMenu.h"
#include "PowerUp.h"
#include "Player.h"

class Game;

enum GameDifficulty : unsigned char
{
	Easy,
	Medium,
	Impossible,

	GAME_DIFFICULTY_COUNT,
};

class GameLevel : public Level
{
private:
	enum  State : unsigned char
	{
		Active,
		Paused,
		Ended,
	};

public:
	enum class LevelMap : unsigned char {
		Infinite,
		Fixed,
	};

public:
	GameLevel() = default;

	void Init(Game* game) override;
	void Update(Game* game) override;
	void Draw(Game* game) override;

private:
	void SpawnEnemy(Game* game);
	void SpawnPowerUp(Game* game, const Vec2& position);
	void EndLevel();

public:
	static constexpr float time_limit = 120.0f;
	Player player;
	Enemy enemies[10000];
	unsigned int enemies_alive{ 0 };
	TileMap level_map{};
	LevelMap level_to_load{LevelMap::Infinite};
	int score{0};
	PowerUp power_ups[10];
	int active_power_ups{0};
	GameDifficulty difficulty;

	bool loaded = false;

private:
	// ReSharper disable once CppUninitializedNonStaticDataMember
	TileMap level_map_{};
	HUD hud_{};
	PauseMenu pause_menu_;
	EndGameMenu end_game_menu_;

	float run_time_{};
	State state_{ Active };

	float last_enemy_spawn_{0};

	// Hacky but I don't want to create an input system.
	bool esc_pressed_{ false };
	bool esc_pressed_last_frame_{ false };

    std::random_device r_;
    std::default_random_engine random_engine_;
	std::uniform_int_distribution<int> power_up_drop_roll_;
};
