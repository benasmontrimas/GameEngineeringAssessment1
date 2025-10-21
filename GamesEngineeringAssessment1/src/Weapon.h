#pragma once
#include <string>
#include "Sprite.h"

// need to spawn a collider on attack, then depending on the attack need to keep track when
// we are attacking.

class Game;
class Enemy;

// Player will do the checks for these, if active pressed all active weapons attack,
// else if player not stunned, check all auto weapons and if off cooldown, attack.
enum class WeaponType {
	AutoAttack,
	Active,
};

class Weapon {
public:
	std::string name{};
	std::string description{};

protected:
	WeaponType type{};

	float cooldown{};
	float last_attack{};
	float damage{};

	float attack_duration{};
	float current_attack_time{};

	Sprite attack_animation{};
	Sprite sprite{};
	Sprite icon{};

	bool is_attacking{};

	int enemies_hit_count{};
	Enemy* enemies_hit[100]; // Arbitraty sized 100 array, assuming were never hitting more than 100 enemies;

public:
	virtual void Attack() = 0;
	virtual void Update(Game* game) = 0;
};