#include <fstream>

#include "MainMenuLevel.h"
#include "Game.h"
#include "GameLevel.h"

void MainMenuLevel::Init(Game* game) {
	bg_.Init(game, "resources/LevelLayouts/MainMenu.txt");

	game->camera.SetFollow(nullptr);
	game->camera.position = Vec2{static_cast<float>(game->window_width) / 2.0f, static_cast<float>(game->window_height) / 2.0f};

	header_.Init(1);
	header_.images[0] = MainMenuHeader;
	header_.depth = INT_MIN;

	constexpr float button_x = 200.0f;

	load_button_.Init(game);
	load_button_.position = Vec2{.x = button_x, .y = 380.0f};

	play_infinite_button_.Init(game);
	play_infinite_button_.position = Vec2{.x = button_x, .y = 440.0f};

	play_fixed_button_.Init(game);
	play_fixed_button_.position = Vec2{.x = button_x, .y = 500.0f};

	difficulty_button_.Init(game);
	difficulty_button_.position = Vec2{ .x = button_x, .y = 560.0f };

	quit_button_.Init(game);
	quit_button_.position = Vec2{.x = button_x, .y = 620.0f };

	// Reset so our water still plays its animation
	game->game_time_multiplier = 1.0f;
}

void MainMenuLevel::Update(Game* game) {
	bg_.Update(game);

	load_button_.Update(game);
	if (load_button_.IsPressed()) {
		LoadLevel(game);
	}

	play_infinite_button_.Update(game);
	if (play_infinite_button_.IsPressed())
	{
		GameLevel* game_level = new GameLevel;
		game_level->level_to_load = GameLevel::LevelMap::Infinite;
		game_level->difficulty = difficulty;
		if (!game->SetNextLevel(game_level)) delete game_level; // If a level is already queued, we just delete this one
	}

	play_fixed_button_.Update(game);
	if (play_fixed_button_.IsPressed())
	{
		GameLevel* game_level = new GameLevel;
		game_level->level_to_load = GameLevel::LevelMap::Fixed;
		game_level->difficulty = difficulty;
		if (!game->SetNextLevel(game_level)) delete game_level; // If a level is already queued, we just delete this one
	}

	difficulty_button_.Update(game);
	if (difficulty_button_.IsPressed()) {
		difficulty = static_cast<GameDifficulty>((difficulty + 1) % GAME_DIFFICULTY_COUNT);
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

	const int header_x = (static_cast<int>(game->window_width) / 2) - (static_cast<int>(game->images[header_.images[0]].width) / 2);
	game->DrawSpriteScreenSpace(header_, {.x = static_cast<float>(header_x), .y = 50.0f });

	const std::string difficulty_string[GAME_DIFFICULTY_COUNT] = {"Easy", "Medium", "No Chance"};

	load_button_.Draw(game, "Load");
	play_infinite_button_.Draw(game, "Infinite");
	play_fixed_button_.Draw(game, "Fixed");
	difficulty_button_.Draw(game, difficulty_string[difficulty]);
	quit_button_.Draw(game, "Quit");

	constexpr Vec2 instructions_position{ 750.0f, 400.0f};
	game->font16.DrawString(game, "WASD or Arrow Keys to move", instructions_position);
	game->font16.DrawString(game, "Space to attack with active", instructions_position + Vec2{0, 40});
	game->font16.DrawString(game, "ESCAPE to pause", instructions_position + Vec2{0, 80});
	game->font16.DrawString(game, "Q to set your health to 0 (die)", instructions_position + Vec2{0, 120});
	game->font16.DrawString(game, "I to toggle Invincibility", instructions_position + Vec2{0, 160});

}

void MainMenuLevel::LoadLevel(Game* game)
{
	const std::string filename{ "Saves/Save.sav" };
	std::fstream fs{ filename, std::fstream::binary | std::fstream::in };

	if (!fs.is_open()) {
		std::cout << "Failed opening save file for loading!\n";
	}
	else {
		GameLevel* game_level = new GameLevel;
		// Did not consider vtables. Need to skip past to first data variable.
		constexpr int data_offset = offsetof(GameLevel, GameLevel::player);
		// Also need to skip loading the players weapons vtables.
		constexpr int sword_vtable_offset = data_offset + offsetof(Player, Player::sword);
		constexpr int aoe_vtable_offset = data_offset + offsetof(Player, Player::aoe);

		const uintptr_t sword_vtable_ptr = reinterpret_cast<uintptr_t*>(&game_level->player.sword)[0];
		const uintptr_t aoe_vtable_ptr = reinterpret_cast<uintptr_t*>(&game_level->player.aoe)[0];

		int v_table_size = sizeof(void*);
		// read into the data ignoring the game level vtable
		fs.read(&reinterpret_cast<char*>(game_level)[data_offset], sizeof(GameLevel) - data_offset);

		uintptr_t* sword_v_table_location = reinterpret_cast<uintptr_t*>(&reinterpret_cast<char*>(game_level)[sword_vtable_offset]);
		sword_v_table_location[0] = sword_vtable_ptr;
		
		uintptr_t* aoe_v_table_location = reinterpret_cast<uintptr_t*>(&reinterpret_cast<char*>(game_level)[aoe_vtable_offset]);
		aoe_v_table_location[0] = aoe_vtable_ptr;

		game_level->loaded = true;
		if (!game->SetNextLevel(game_level)) delete game_level;
	}
}