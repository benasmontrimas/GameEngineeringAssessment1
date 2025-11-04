#pragma once
#include <string>
#include "Sprite.h"

// need to spawn a collider on attack, then depending on the attack need to keep track when
// we are attacking.

class Game;
class Enemy;

// Player will do the checks for these, if active pressed all active weapons attack,
// else if player not stunned, check all auto weapons and if off cooldown, attack.
enum class WeaponType : unsigned char {
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

	Sprite icon{};
public:
	virtual void Init(Game* game) = 0;
	virtual void Attack(Game* game) = 0;
	virtual void Update(Game* game) = 0;
	virtual void Draw(Game* game) = 0;
};