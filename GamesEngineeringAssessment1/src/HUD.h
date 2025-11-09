#pragma once

#include "Sprite.h"

class Game;

// TODO: Need to add player health bar, player kills, fps
class HUD {
public:
	void Init(Game* game);
	void Update(const Game* game);
	void Draw(Game* game, float elapsed_seconds) const;

	Sprite sword_border{};
	Sprite sword_icon{};

	Sprite aoe_border{};
	Sprite aoe_icon{};
};