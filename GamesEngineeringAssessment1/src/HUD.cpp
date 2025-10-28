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

void HUD::Draw(Game* game) const
{
	auto [x, y] = game->camera.position - Vec2{.x = static_cast<float>(game->window_width) / 2.0f ,
		.y = static_cast<float>(game->window_height) / 2.0f };

	game->DrawSprite(weapon_1_border, Vec2{.x = 10 + x, .y = 10 + y});
	game->DrawSprite(weapon_1_icon, Vec2{.x = 10 + x, .y = 10 + y });

	game->DrawSprite(weapon_2_border, Vec2{.x = 10 + x, .y = 10 + 64 + 10 + y});
}
