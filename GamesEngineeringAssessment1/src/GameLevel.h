#pragma once
#include "Enemy.h"
#include "HUD.h"
#include "Level.h"
#include "Map.h"
#include "PauseMenu.h"
#include "Player.h"

class Game;

class GameLevel : public Level
{
	enum State : unsigned char
	{
		Active,
		Paused,
		Ended,
	};

public:
	GameLevel() = default;

	void Init(Game* game) override;
	void Update(Game* game) override;
	void Draw(Game* game) override;
	void Shutdown(Game* game) override;

private:
	void SpawnEnemy(Game* game);

public:
	static constexpr float time_limit = 120.0f;

private:
	Player player_;
	// ReSharper disable once CppUninitializedNonStaticDataMember
	Enemy enemies_[10000];
	TileMap level_map_{};
	HUD hud_{};
	PauseMenu pause_menu_;

	float run_time_{};
	unsigned int enemies_alive_{ 0 };
	State state_{ Active };

	float last_enemy_spawn_{0};

	// Hacky but I don't want to create an input system.
	bool esc_pressed_{ false };
	bool esc_pressed_last_frame_{ false };
};
