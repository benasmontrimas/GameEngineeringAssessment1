#pragma once

#include "GamesEngineeringBase.h"
#include "CustomBase.h"

class Player {
public:
	Player() : position(Vec2{}) {}

	void Init();

	Vec2 position;
	GamesEngineeringBase::Image sprite;
};