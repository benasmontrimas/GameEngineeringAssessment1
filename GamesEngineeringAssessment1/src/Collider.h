#pragma once
#include "CustomBase.h"
// Do I need anything but sphere colliders?

class Collider {
public:
	[[nodiscard]] bool CheckCollision(const Collider& other) const;
	// Return the collision direction vec2, else return empty vec2 if no collision.
	[[nodiscard]] Vec2 CheckCollisionSquare(const Vec2& pos, float size) const;

	Vec2 center;
	float radius;
};