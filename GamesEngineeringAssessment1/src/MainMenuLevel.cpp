#include "MainMenuLevel.h"
#include "Game.h"

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
	play_button_.Update(game);
	settings_button_.Update(game);
	quit_button_.Update(game);
}

void MainMenuLevel::Draw(Game* game) {
	game->DrawSpriteScreenSpace(header_, {.x = 50.0f, .y = 50.0f });
	play_button_.Draw(game);
	settings_button_.Draw(game);
	quit_button_.Draw(game);
}

void MainMenuLevel::Shutdown(Game* game) {

}