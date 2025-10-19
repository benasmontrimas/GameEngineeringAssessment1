#pragma once

#include "GamesEngineeringBase.h"
#include "CustomBase.h"
#include "Sprite.h"

class Game;

// TODO: Create a weapon class, add slots for the weapons on the player, and as they equip them,
// Loop this list and activate them. Simple.
class Player {
public:
	Player() = default;

	void Init(Game* game);
	void Update(Game* game);

	Vec2 position;
	Sprite sprite;
};