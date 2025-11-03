#pragma once

#include "CustomBase.h"
#include "Sprite.h"
#include "Collider.h"

class Game;

// TODO: Create a weapon class, add slots for the weapons on the player, and as they equip them,
// Loop this list and activate them. Simple.
class Player {
	enum class State : unsigned char {
		Idle,
		Walking,
		Dying,
	};

public:
	Player() = default;

	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game) const;

	void HandleInput(const Game* game);
	void SetState(State new_state);

	State state{};

	Sprite sprite{};
	Sprite walking_sprite{};
	Sprite dying_sprite{};

	Vec2 position{0, 0};
	Collider collider{};

	// Add weapons, one projectile one aoe. Need to get distance to enemies to attack closest.

	float movement_speed{ 150 };
	Vec2 movement_direction{};
};