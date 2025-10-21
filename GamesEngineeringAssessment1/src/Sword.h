#pragma once

#include "Weapon.h"

class Sword : public Weapon {
public:
	Sword();

	void Attack() override;
	void Update(Game* game) override;
private:
};