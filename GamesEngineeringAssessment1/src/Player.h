#pragma once

#include "GamesEngineeringBase.h"
#include "CustomBase.h"
#include "Sprite.h"
#include "Collider.h"

class Game;

// TODO: Create a weapon class, add slots for the weapons on the player, and as they equip them,
// Loop this list and activate them. Simple.
class Player {
public:
	Player() = default;

	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game);

	Vec2 position{};
	Sprite sprite{};
	Collider collider{};

	// Add weapons, one projectile one aoe. Need to get distance to enemies to attack closest.

	float movement_speed{ 150 };
	Vec2 movement_direction{};
};