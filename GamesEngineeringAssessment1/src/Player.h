#pragma once

#include "Vec2.h"
#include "Sprite.h"
#include "Collider.h"
#include "Sword.h"
#include "AOEWeapon.h"

class Game;

// TODO: Create a weapon class, add slots for the weapons on the player, and as they equip them,
// Loop this list and activate them. Simple.
class Player {
public:
	enum class State : unsigned char {
		Idle,
		Walking,
		Dying,
		Dead,
	};

public:
	Player() = default;

	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game);

	void HandleInput(const Game* game);
	void SetState(State new_state);

	void Hit(float damage);

	State state{};

	Sprite sprite{};
	Sprite walking_sprite{};
	Sprite dying_sprite{};

	Vec2 last_position{};
	Vec2 position{0, 0};
	Collider collider{};

	// Add weapons, one projectile one aoe. Need to get distance to enemies to attack closest.

	float max_health{150};
	float current_health{};
	float time_dying{};

	float movement_speed{ 150 };
	Vec2 movement_direction{};

	float collision_cooldown{1.0f};
	float time_since_last_collision{0.0f};

	// If I had weapons the player could choose from and select i would do it this way. But we have 2 weapons that are set.
	// Weapon* weapons[2] {nullptr, nullptr};
	Sword sword;
	AOEWeapon aoe;

	float power_up_time_left{};

	bool i_pressed_last_frame = false;
	bool is_invincible = false;
};