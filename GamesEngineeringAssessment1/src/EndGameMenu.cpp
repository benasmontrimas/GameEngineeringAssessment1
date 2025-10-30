#include "EndGameMenu.h"

#include "Game.h"
#include "GameLevel.h"
#include "MainMenuLevel.h"

void EndGameMenu::Init(Game* game)
{
	background_.Init(1);
	// background_.images[0] = &game->images[PopupMenuBackground];

	// TODO: Set positions.
	restart_button_.Init(game, "Restart");
	continue_button_.Init(game, "Continue");
}

void EndGameMenu::Update(Game* game)
{
	// background_.Update(game);

	restart_button_.Update(game);
	if (restart_button_.IsPressed())
	{
		GameLevel* game_level = new GameLevel;
		if (!game->SetNextLevel(game_level)) delete game_level;
	}

	continue_button_.Update(game);
	if (continue_button_.IsPressed())
	{
		MainMenuLevel* main_menu = new MainMenuLevel;
		if (!game->SetNextLevel(main_menu)) delete main_menu;
	}
}

void EndGameMenu::Draw(Game* game)
{
	const unsigned int bg_x = game->window_width / 2 - 150;
	const unsigned int bg_y = game->window_height / 2 - 150;
	game->DrawSpriteScreenSpace(background_, Vec2{.x = static_cast<float>(bg_x), .y = static_cast<float>(bg_y)});

	// Need to draw text for score and time survived

	restart_button_.Draw(game);
	continue_button_.Draw(game);
}


