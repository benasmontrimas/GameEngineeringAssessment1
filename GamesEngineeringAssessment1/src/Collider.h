#pragma once
#include "CustomBase.h"
// Do i need anything but sphere coliders?

class Collider {
public:
	bool CheckCollision(Collider other);

	Vec2 center;
	float radius;
};