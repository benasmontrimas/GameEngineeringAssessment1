#include "GameLevel.h"
#include "Game.h"

void GameLevel::Init(Game* game)
{
	random_engine_ = std::default_random_engine(r_());
	power_up_drop_roll_ = std::uniform_int_distribution<int>(1, 100);

	// Don't need to initialize most things.
	// Just set camera follow, and load the level from file.
	if (loaded) {
		switch (level_to_load) {
		case LevelMap::Infinite:
			level_map.Init(game, "resources/LevelLayouts/Infinite.txt");
			break;
		case LevelMap::Fixed:
			level_map.Init(game, "resources/LevelLayouts/Fixed.txt");
			break;
		}

		game->camera.SetFollow(&player.position);
		game->camera.position = player.position;

		state_ = Active;
		return;
	}

	active_power_ups = 0;

	player.Init(game);
	// Move player into the map.
	player.position.x = 200;
	player.position.y = 200;

	switch (level_to_load) {
	case LevelMap::Infinite:
		level_map.Init(game, "resources/LevelLayouts/Infinite.txt");
		break;
	case LevelMap::Fixed:
		level_map.Init(game, "resources/LevelLayouts/Fixed.txt");
		break;
	}

	hud_.Init(game);
	pause_menu_.Init(game);
	end_game_menu_.Init(game);

	game->camera.SetFollow(&player.position);
	game->camera.position = player.position;

	state_ = Active;
	score = 0;
}

// Check if escape pressed, if so enter pause, and set multiplier to 0
void GameLevel::Update(Game* game)
{
	esc_pressed_ = game->window.keyPressed(VK_ESCAPE);

	// NOTE: FOR TESTING
	if (game->window.keyPressed('Q')) {
		player.Hit(1000);
	}

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
			EndLevel();
		}

		// Just spawns an enemy every second based on how many seconds passed.
		if (run_time_ > last_enemy_spawn_) {
			float enemy_spawn_divisor = 15;;
			switch (difficulty)
			{
			case Easy:
				enemy_spawn_divisor = 15;
				break;
			case Medium:
				enemy_spawn_divisor = 10;
				break;
			case  Impossible:
				enemy_spawn_divisor = 1;
				break;
			default: ;
			}

			const int enemies_to_spawn = static_cast<int>(run_time_ / enemy_spawn_divisor);
			for (int i = 0; i < enemies_to_spawn; i++) {
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
		end_game_menu_.Update(game);

		break;
	}

	esc_pressed_last_frame_ = esc_pressed_;

	// can still update all this as delta_time should be 0, at least the frame after we pause it should be.
	player.Update(game);
	if (player.state == Player::State::Dead) {
		EndLevel();
	}

	// Check if enemies died.
	// NOTE: the bombers dying by attacking still adds to score.
	// Backwards for removal
	for (int i = static_cast<int>(enemies_alive) - 1; i >= 0; i--)
	{
		enemies[i].Update(game);
		// swap and pop
		if (enemies[i].state == Enemy::Dead) {
			// Roll random chance, if hit, spawn power-up, if power-up picked up, set player power-up timer to 10
			const int roll = power_up_drop_roll_(random_engine_);
			if (roll == 1) {
				SpawnPowerUp(game, enemies[i].position);
			}

			enemies[i] = enemies[enemies_alive - 1];
			score++;
			enemies_alive--;
		}
	}

	for (int i = active_power_ups - 1; i >= 0; i--) {
		power_ups[i].Update(game);

		if (power_ups[i].is_expired) {
			power_ups[i] = power_ups[active_power_ups - 1];
			active_power_ups--;
		}
	}

	level_map.Update(game);

	hud_.Update(game);
}

void GameLevel::Draw(Game* game)
{
	switch (state_) {
	case (Paused):
		pause_menu_.Draw(game);
		break;
	case Ended:
		end_game_menu_.Draw(game);
		break;
	default: ;
	}

	//// NEED TO DRAW BEFORE PLAYER FOR TRANSPARENCY ON AOE ATTACK
	level_map.Draw(game);

	hud_.Draw(game, run_time_);
	player.Draw(game);

	for (unsigned int i = 0; i < enemies_alive; i++)
	{
		enemies[i].Draw(game);
	}

	for (int i = 0; i < active_power_ups; i++) {
		power_ups[i].Draw(game);
	}
}

void GameLevel::SpawnEnemy(Game* game)
{
	if (enemies_alive >= 10000) return;

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

	enemies[enemies_alive].Init(game, static_cast<EnemyType>(rand() % ENEMY_TYPE_COUNT));
	enemies[enemies_alive].position = player.position + (normalized_spawn_vector * abs(multiplier));
	enemies_alive++;
}

void GameLevel::SpawnPowerUp(Game* game, const Vec2& position) {
	if (active_power_ups > 10) return; // If were already at max don't spawn more.
	power_ups[active_power_ups].Init(game);
	power_ups[active_power_ups].position = position;
	active_power_ups++;
}

void GameLevel::EndLevel() {
	end_game_menu_.time_survived = static_cast<int>(run_time_);
	end_game_menu_.final_score = score;
	state_ = Ended;
}