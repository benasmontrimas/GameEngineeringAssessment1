#pragma once
#include "Sprite.h"

// need to spawn a collider on attack, then depending on the attack need to keep track when
// we are attacking.

class Game;

class Weapon {
public:
	virtual ~Weapon() = default;

	virtual void Init(Game* game) = 0;
	virtual void Attack(Game* game) = 0;
	virtual void Update(Game* game) = 0;
	virtual void Draw(Game* game) = 0;

public:
	float cooldown{};
	float last_attack{};

	float damage{};

	Sprite icon{};
};