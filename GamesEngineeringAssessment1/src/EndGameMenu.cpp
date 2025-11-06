#include "EndGameMenu.h"

#include "Game.h"
#include "GameLevel.h"
#include "MainMenuLevel.h"

void EndGameMenu::Init(Game* game)
{
	background_.Init(1);
	background_.images[0] = &game->images[PopupMenuBG];
	background_.depth = INT_MIN + 2;

	const unsigned int button_x = game->window_width / 2 - Button::width / 2;
	const unsigned int half_y = game->window_height / 2 - Button::height / 2;

	restart_button_.Init(game, "Restart");
	restart_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y) };

	continue_button_.Init(game, "Continue");
	continue_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y + Button::height + 10) };
}

void EndGameMenu::Update(Game* game)
{
	background_.Update(game);

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
	std::string time_survived_string = "Time: " + std::to_string(time_survived);
	std::string score_text = "Score: " + std::to_string(final_score);

	int text_x = (game->window_width / 2);
	int text_y = (game->window_height / 2) - 125;

	game->font32.DrawString(game, time_survived_string, Vec2{float(text_x - ((32 * time_survived_string.length()) * 0.7) / 2), float(text_y)});
	game->font32.DrawString(game, score_text, Vec2{float(text_x - ((32 * score_text.length()) * 0.7) / 2), float(text_y) + 40.0f});

	restart_button_.Draw(game);
	continue_button_.Draw(game);
}


