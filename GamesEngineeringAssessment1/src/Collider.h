#pragma once
#include "CustomBase.h"
// Do I need anything but sphere colliders?

class Collider {
public:
	[[nodiscard]] bool CheckCollision(const Collider& other) const;

	Vec2 center;
	float radius;
};