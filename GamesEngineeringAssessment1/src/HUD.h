#pragma once

#include "Sprite.h"

class Game;

class HUD {
public:
	void Init(Game* game);
	void Draw(Game* game);

	Sprite weapon_1_border;
	Sprite weapon_1_icon;

	Sprite weapon_2_border;
};