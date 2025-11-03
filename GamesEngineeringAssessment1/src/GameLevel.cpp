#include "GameLevel.h"
#include "Game.h"
#include "MainMenuLevel.h"

void GameLevel::Init(Game* game)
{
	player_.Init(game);
	level_map_.Init(game);
	hud_.Init(game);
	pause_menu_.Init(game);

	game->camera.SetFollow(&player_.position);
	game->camera.position = player_.position;

	state_ = Active;
}

// Check if escape pressed, if so enter pause, and set multiplier to 0
void GameLevel::Update(Game* game)
{
	esc_pressed_ = game->window.keyPressed(VK_ESCAPE);

	switch (state_)
	{
	case Active:
		game->game_time_multiplier = 1.0f;

		if (esc_pressed_ and !esc_pressed_last_frame_)
		{
			state_ = Paused;
		}

		run_time_ += game->game_time;

		if (run_time_ > time_limit)
		{
			state_ = Ended;
		}

		// Just spawns an enemy every seconds based on how many seconds passed.
		if (run_time_ > last_enemy_spawn_) {
			for (int i = 0; i < static_cast<int>(run_time_); i++) {
				SpawnEnemy(game);
			}

			last_enemy_spawn_ = ceil(run_time_);
		}

		break;
	case Paused:
		game->game_time_multiplier = 0.0f;

		if (esc_pressed_ and !esc_pressed_last_frame_)
		{
			state_ = Active;
		}

		pause_menu_.Update(game);
		if (pause_menu_.IsContinue())
		{
			state_ = Active;
		}

		break;
	case Ended:
		game->game_time_multiplier = 0.0f;

		// Create End Game menu

		break;
	}

	esc_pressed_last_frame_ = esc_pressed_;

	// can still update all this as delta_time should be 0, at least the frame after we pause it should be.
	player_.Update(game);

	for (unsigned int i = 0; i < enemies_alive_; i++)
	{
		enemies_[i].Update(game);
	}

	level_map_.Update(game);

	hud_.Update(game);
}

void GameLevel::Draw(Game* game)
{
	if (state_ == Paused)
	{
		pause_menu_.Draw(game);
	}

	hud_.Draw(game, run_time_);
	player_.Draw(game);

	for (unsigned int i = 0; i < enemies_alive_; i++)
	{
		enemies_[i].Draw(game);
	}

	level_map_.Draw(game);
}

void GameLevel::Shutdown(Game* game)
{

}

void GameLevel::SpawnEnemy(Game* game)
{
	if (enemies_alive_ >= 10000) return;

	Vec2 spawn_vector{};
	const int rand_x = rand() % 2000;
	const int rand_y = rand() % 2000;
	spawn_vector.x = static_cast<float>(rand_x - 1000) / 1000.0f;
	spawn_vector.y = static_cast<float>(rand_y - 1000) / 1000.0f;

	const Vec2 normalized_spawn_vector = NormalizeVec2(spawn_vector);
	float multiplier = 0.0f;
	if (abs(1200 / normalized_spawn_vector.x) < abs(800 / normalized_spawn_vector.y)) {
		// sides closer
		multiplier = static_cast<float>(1200) / normalized_spawn_vector.x;
	}
	else {
		// top/bottom closer
		multiplier = static_cast<float>(800) / normalized_spawn_vector.y;
	}

	enemies_[enemies_alive_].position = player_.position + (normalized_spawn_vector * abs(multiplier));
	enemies_[enemies_alive_].Init(game, Zombie);
	enemies_[enemies_alive_].player = &player_;
	enemies_alive_++;
}

