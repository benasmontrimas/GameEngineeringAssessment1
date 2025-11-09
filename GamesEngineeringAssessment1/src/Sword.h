#pragma once

#include "Vec2.h"
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
	int projectile_count_ = 0;
	static constexpr int max_projectile_count = 100;
	Projectile projectiles_[max_projectile_count] = {};
	Vec2 attack_direction_{.x = 1, .y = 0};
};