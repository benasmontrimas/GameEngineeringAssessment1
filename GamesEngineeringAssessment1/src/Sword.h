#pragma once

#include "CustomBase.h"
#include "Projectile.h"
#include "Weapon.h"

class Game;

class Sword : public Weapon {
public:
	void Init(Game* game) override;
	void Attack(Game* game) override;
	void Update(Game* game) override;
	void Draw(Game* game) override;

	void GetAttackDirection(const Game* game);

private:
	int projectile_count = 0;
	Projectile projectiles[100];
	Vec2 attack_direction_{.x = 1, .y = 0};
};