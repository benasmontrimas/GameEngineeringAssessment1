#pragma once

#include "Sprite.h"

class Game;

// TODO: Need to add player health bar, player kills, fps
class HUD {
public:
	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game, float elapsed_seconds) const;

	Sprite weapon_1_border;
	Sprite weapon_1_icon;

	Sprite weapon_2_border;
};