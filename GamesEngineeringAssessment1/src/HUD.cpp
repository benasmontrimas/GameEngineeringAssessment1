#include "HUD.h"
#include "Game.h"
#include "GameLevel.h"

void HUD::Init(Game* game) {
	sword_border.Init(1);
	sword_border.images[0] = ItemBorder;
	sword_border.depth = INT_MIN;

	sword_icon.Init(1);
	sword_icon.images[0] = SwordIcon;
	sword_icon.depth = INT_MIN;

	aoe_border.Init(1);
	aoe_border.images[0] = ItemBorder;
	aoe_border.depth = INT_MIN;

	aoe_icon.Init(1);
	aoe_icon.images[0] = AOEIcon;
	aoe_icon.depth = INT_MIN;
}

// Need an update function which checks for weapons equipped and get the correct icon for them.
void HUD::Update(const Game* game)
{
	sword_icon.Update(game);
	aoe_icon.Update(game);
}


void HUD::Draw(Game* game, const float elapsed_seconds) const
{
	const GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
	const Player& player = level->player;
	const Sword& sword = player.sword;
	const AOEWeapon& aoe = player.aoe;

	constexpr Vec2 sword_hud_position = Vec2{.x = 10, .y = 10};
	game->DrawSpriteScreenSpace(sword_border, sword_hud_position);
	game->DrawSpriteScreenSpace(sword_icon, sword_hud_position);
	const float sword_cooldown_width = 64 * (min(sword.last_attack, sword.cooldown) / sword.cooldown);
	game->DrawRectScreenSpace(sword_hud_position, Vec2{sword_cooldown_width, 64}, INT_MIN + 1, 200, 200, 200);

	constexpr Vec2 aoe_hud_position = Vec2{.x = 10, .y = 10 + 64 + 10};
	game->DrawSpriteScreenSpace(aoe_border, aoe_hud_position);
	game->DrawSpriteScreenSpace(aoe_icon, aoe_hud_position);
	const float aoe_cooldown_width = 64 * (min(aoe.last_attack, aoe.cooldown) / aoe.cooldown);
	game->DrawRectScreenSpace(aoe_hud_position, Vec2{aoe_cooldown_width, 64}, INT_MIN + 1, 200, 200, 200);


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
	constexpr float time_text_width = ((32 * 5 * 0.6) / 2);
	const Vec2 time_text_position = Vec2{.x = (static_cast<float>(game->window_width) / 2) - time_text_width, .y = 50.0f};
	game->font32.DrawString(game, minutes_string + ":" + seconds_string, time_text_position);

	game->font16.DrawString(game, "FPS: " + std::to_string(game->fps), Vec2{80.0f, 10.0f});

	game->DrawRect(player.position + Vec2{0, 64}, Vec2{48, 5}, 10, 255, 0, 0);

	const float health_bar_end = 48.0f * (player.current_health / player.max_health);
	game->DrawRect(player.position + Vec2{0, 64}, Vec2{health_bar_end, 5}, 0, 0, 255, 0);

	const Vec2 score_position{.x = static_cast<float>(game->window_width) - 200, .y = 50.0f};
	game->font32.DrawString(game, "Score: " + std::to_string(level->score), score_position);

	// Draw a little I in the corner to show that you are invincible.
	if (player.is_invincible) {
		game->font32.DrawString(game, "I", Vec2{10, static_cast<float>(game->window_height) - 40});
	}
}
