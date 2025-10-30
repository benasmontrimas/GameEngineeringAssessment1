#include "HUD.h"
#include "Game.h"

void HUD::Init(Game* game) {
	weapon_1_border.Init(1);
	weapon_1_border.images[0] = &game->images[ItemBorder];
	weapon_1_border.depth = INT_MIN;

	weapon_1_icon.Init(1);
	weapon_1_icon.images[0] = &game->images[Weapon1];
	weapon_1_icon.depth = INT_MIN;

	weapon_2_border.Init(1);
	weapon_2_border.images[0] = &game->images[ItemBorder];
	weapon_2_border.depth = INT_MIN;
}

// Need an update function which checks for weapons equipped and get the correct icon for them.
void HUD::Update(Game* game)
{
	weapon_1_icon.Update(game);
}


void HUD::Draw(Game* game, float elapsed_seconds) const
{
	game->DrawSpriteScreenSpace(weapon_1_border, Vec2{.x = 10, .y = 10});
	game->DrawSpriteScreenSpace(weapon_1_icon, Vec2{.x = 10, .y = 10});

	game->DrawSpriteScreenSpace(weapon_2_border, Vec2{.x = 10, .y = 10 + 64 + 10});

	// Assuming the hud exists if this exists.

	int seconds = static_cast<int>(elapsed_seconds);
	const int minutes = seconds / 60;
	seconds = seconds % 60;
	
	std::string seconds_string = "00";
	std::string minutes_string = "00";
	
	seconds_string[0] = std::to_string(seconds / 10)[0];
	seconds_string[1] = std::to_string(seconds % 10)[0];
	
	minutes_string[0] = std::to_string(minutes / 10)[0];
	minutes_string[1] = std::to_string(minutes % 10)[0];
	
	// I can work out text width very easily (32 * chars * 0.6) as 32 is the width of each char in my map, and 0.6 is the multiplier.
	game->font32.DrawString(game, minutes_string + ":" + seconds_string, Vec2{.x = 800.0f, .y = 50.0f });

	game->font16.DrawString(game, "FPS: " + std::to_string(game->fps), Vec2{80.0f, 10.0f});
}
