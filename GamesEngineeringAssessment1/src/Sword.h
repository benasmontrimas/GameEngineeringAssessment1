#pragma once

#include "CustomBase.h"
#include "Weapon.h"

class Sword : public Weapon {
public:
	Sword();

	void Attack() override;
	void Update(Game* game) override;

	void GetAttackDirection(const Game* game);

private:
	Vec2 attack_direction_{.x = 1, .y = 0};
};