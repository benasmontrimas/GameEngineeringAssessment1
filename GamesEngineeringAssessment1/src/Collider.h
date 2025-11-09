#pragma once
#include "Vec2.h"

// More accurately should be named circle collider.
class Collider {
public:
	[[nodiscard]] bool CheckCollision(const Collider& other) const;
	// Return the collision direction vec2, else return zero vec2 if no collision.
	[[nodiscard]] Vec2 CheckCollisionSquare(const Vec2& pos, float size) const;

	Vec2 center;
	float radius;
};