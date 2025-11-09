#include "EndGameMenu.h"

#include "Game.h"
#include "GameLevel.h"
#include "MainMenuLevel.h"

void EndGameMenu::Init(Game* game)
{
	background_.Init(1);
	background_.images[0] = PopupMenuBG;
	background_.depth = INT_MIN + 2;

	const unsigned int button_x = game->window_width / 2 - Button::width / 2;
	const unsigned int half_y = game->window_height / 2 - Button::height / 2;

	restart_button_.Init(game);
	restart_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y) };

	main_menu_button_.Init(game);
	main_menu_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y + Button::height + 10) };
}

void EndGameMenu::Update(Game* game)
{
	background_.Update(game);

	restart_button_.Update(game);
	if (restart_button_.IsPressed())
	{
		const GameLevel* current_level = dynamic_cast<GameLevel*>(game->GetLevel());

		GameLevel* game_level = new GameLevel;
		game_level->level_map = current_level->level_map;
		game_level->difficulty = current_level->difficulty;
		if (!game->SetNextLevel(game_level)) delete game_level;
	}

	main_menu_button_.Update(game);
	if (main_menu_button_.IsPressed())
	{
		const GameLevel* current_level = dynamic_cast<GameLevel*>(game->GetLevel());
		MainMenuLevel* main_menu = new MainMenuLevel;
		main_menu->difficulty = current_level->difficulty;
		if (!game->SetNextLevel(main_menu)) delete main_menu;
	}
}

void EndGameMenu::Draw(Game* game) const
{
	const unsigned int bg_x = game->window_width / 2 - 150;
	const unsigned int bg_y = game->window_height / 2 - 150;
	game->DrawSpriteScreenSpace(background_, Vec2{.x = static_cast<float>(bg_x), .y = static_cast<float>(bg_y)});

	// Need to draw text for score and time survived
	const std::string time_survived_string = "Time: " + std::to_string(time_survived);
	const std::string score_text = "Score: " + std::to_string(final_score);

	const float text_x = static_cast<float>(game->window_width) / 2.0f;
	const float text_y = (static_cast<float>(game->window_height) / 2.0f) - 125.0f;

	const float time_survived_text_width = 32.0f * static_cast<float>(time_survived_string.length()) * 0.6f;
	const Vec2 time_survived_pos = Vec2{ text_x - (time_survived_text_width / 2), static_cast<float>(text_y) };
	game->font32.DrawString(game, time_survived_string, time_survived_pos);

	const float score_text_width = 32.0f * static_cast<float>(score_text.length()) * 0.6f;
	const Vec2 score_text_pos = Vec2{text_x - (score_text_width / 2), static_cast<float>(text_y) + 40.0f };
	game->font32.DrawString(game, score_text, score_text_pos);

	restart_button_.Draw(game, "Restart");
	main_menu_button_.Draw(game, "Main Menu");
}


