#include "HUD.h"
#include "Game.h"

void HUD::Init(Game* game) {
	weapon_1_border.Init(1);
	weapon_1_border.images[0] = &game->images[ItemBorder];
	weapon_1_border.depth = INT_MIN;

	weapon_1_icon.Init(1);
	weapon_1_icon.images[0] = &game->images[AOEWeapon];
	weapon_1_icon.depth = INT_MIN;

	weapon_2_border.Init(1);
	weapon_2_border.images[0] = &game->images[ItemBorder];
	weapon_2_border.depth = INT_MIN;
}

// Need an update function which checks for weapons equiped and get the correct icon for them.

void HUD::Draw(Game* game) {
	Vec2 camera_position = game->camera.position - Vec2{ float(game->window_width) / 2.0f , float(game->window_height) / 2.0f };

	game->DrawImage(weapon_1_border, Vec2{ 10 + camera_position.x, 10 + camera_position.y});
	game->DrawImage(weapon_1_icon, Vec2{ 10 + camera_position.x, 10 + camera_position.y });

	game->DrawImage(weapon_2_border, Vec2{ 10 + camera_position.x, 10 + 64 + 10 + camera_position.y});
}