#include "MainMenuLevel.h"
#include "Game.h"
#include "GameLevel.h"

void MainMenuLevel::Init(Game* game) {
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
}

void MainMenuLevel::Update(Game* game) {
	// All these actions need to queue (well we can probably quit now), but need to send message to game to tell it this is what it should do after this frame ends.
	play_button_.Update(game);
	if (play_button_.IsPressed())
	{
		// Play
		GameLevel* game_level = new GameLevel;
		if (!game->SetNextLevel(game_level)) delete game_level; // If a level is already queued, we just delete this one, next frame the level should already change.
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
	int header_x = (game->window_width / 2) - (header_.images[0]->width / 2);
	game->DrawSpriteScreenSpace(header_, {.x = static_cast<float>(header_x), .y = 50.0f });
	play_button_.Draw(game);
	settings_button_.Draw(game);
	quit_button_.Draw(game);
}

void MainMenuLevel::Shutdown(Game* game) {

}