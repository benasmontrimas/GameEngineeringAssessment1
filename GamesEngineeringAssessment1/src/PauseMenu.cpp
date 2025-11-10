#include <fstream>

#include "PauseMenu.h"

#include "Game.h"
#include "GameLevel.h"
#include "MainMenuLevel.h"

void PauseMenu::Init(Game* game)
{
	background_.Init(1);
	background_.images[0] = PopupMenuBG; // Need to create a background
	background_.depth = INT_MIN + 2;

	const unsigned int button_x = game->window_width / 2 - Button::width / 2;
	const unsigned int half_y = game->window_height / 2 - Button::height / 2;

	continue_button_.Init(game);
	continue_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y - Button::height - 10) };

	save_button_.Init(game);
	save_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y) };

	quit_button_.Init(game);
	quit_button_.position = Vec2{.x = static_cast<float>(button_x), .y = static_cast<float>(half_y + Button::height + 10) };
}

void PauseMenu::Update(Game* game)
{
	background_.Update(game);

	// Parent checks for press.
	continue_button_.Update(game);

	save_button_.Update(game);
	if (save_button_.IsPressed())
	{
		// NOTE: This gives no feedback that the game saved, only prints to console.
		SaveLevel(game);
	}

	quit_button_.Update(game);
	if (quit_button_.IsPressed())
	{
		MainMenuLevel* main_menu = new MainMenuLevel;
		if (!game->SetNextLevel(main_menu)) delete main_menu;
	}
}

void PauseMenu::Draw(Game* game) const
{
	const unsigned int bg_x = game->window_width / 2 - 150;
	const unsigned int bg_y = game->window_height / 2 - 150;
	game->DrawSpriteScreenSpace(background_, Vec2{static_cast<float>(bg_x), static_cast<float>(bg_y)});

	continue_button_.Draw(game, "Continue");
	save_button_.Draw(game, "Save");
	quit_button_.Draw(game, "Quit");
}

bool PauseMenu::IsContinue() const
{
	return continue_button_.IsPressed();
}

void PauseMenu::SaveLevel(const Game* game)
{
	// Need to call save on the game level, don't have this yet.
	const std::string filename{ "Saves/Save.sav" };
	std::fstream fs{ filename, std::fstream::binary | std::fstream::out };

	if (!fs.is_open()) {
		std::cout << "failed opening file for saving!\n";
	}
	else {
		GameLevel* game_level = dynamic_cast<GameLevel*>(game->GetLevel());
		constexpr int data_offset = offsetof(GameLevel, GameLevel::player);
		fs.write(&reinterpret_cast<char*>(game_level)[data_offset], sizeof(GameLevel) - data_offset);
		std::cout << "Saved\n";
		fs.close();
	}
}