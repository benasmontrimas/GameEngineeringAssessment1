#include "MainMenuLevel.h"
#include "Game.h"
#include "GameLevel.h"

void MainMenuLevel::Init(Game* game) {
	bg_.Init(game, "");

	game->camera.SetFollow(nullptr);
	game->camera.position = Vec2{0, 0};

	header_.Init(1);
	header_.images[0] = &game->images[MainMenuHeader];
	header_.depth = INT_MIN;

	const float button_x = static_cast<float>(game->window_width - Button::width) / 2.0f;

	play_button_.Init(game, "Play");
	play_button_.position = Vec2{.x = button_x, .y = 500.0f};

	settings_button_.Init(game, "Settings");
	settings_button_.position = Vec2{.x = button_x, .y = 560.0f };

	quit_button_.Init(game, "Quit");
	quit_button_.position = Vec2{.x = button_x, .y = 620.0f };

	// Reset so our water still plays its animation
	game->game_time_multiplier = 1.0f;
}

void MainMenuLevel::Update(Game* game) {
	bg_.Update(game);

	play_button_.Update(game);
	if (play_button_.IsPressed())
	{
		// Play
		GameLevel* game_level = new GameLevel;
		if (!game->SetNextLevel(game_level)) delete game_level; // If a level is already queued, we just delete this one
	}

	settings_button_.Update(game);
	if (settings_button_.IsPressed())
	{
		// Open Settings
	}

	quit_button_.Update(game);
	if (quit_button_.IsPressed())
	{
		// Quit
		game->running = false;
	}
}

void MainMenuLevel::Draw(Game* game) {
	bg_.Draw(game);

	int header_x = (game->window_width / 2) - (header_.images[0]->width / 2);
	game->DrawSpriteScreenSpace(header_, {.x = static_cast<float>(header_x), .y = 50.0f });

	play_button_.Draw(game);
	settings_button_.Draw(game);
	quit_button_.Draw(game);
}

void MainMenuLevel::Shutdown(Game* game) {

}