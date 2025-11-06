#include "Collider.h"

bool Collider::CheckCollision(const Collider& other) const
{
	const Vec2 distance_vector = other.center - center;
	const float distance_squared = distance_vector.MagSquared();
	return distance_squared < ((other.radius * other.radius) + (radius * radius));
}

Vec2 Collider::CheckCollisionSquare(const Vec2& pos, float size) const
{
	// Check rect dimensions + radius
	if (center.x + radius < pos.x) return {0,0};
	if (center.x - radius > pos.x + size) return {0,0};
	if (center.y + radius < pos.y) return {0,0};
	if (center.y - radius > pos.y + size) return {0,0};

	// check collider center to the 2 rects made if we extend them by the radius (this checks everything but corners.
	if (center.x > pos.x - radius and
		center.x < pos.x + size + radius and
		center.y > pos.y and
		center.y < pos.y + size) {
		// If were to teh left of square center we return right
		if (center.x < pos.x + (size / 2)) return Vec2{1, 0};
		// else return left
		return Vec2{-1, 0};
	}

	if (center.x > pos.x and
		center.x < pos.x + size and
		center.y > pos.y - radius and
		center.y < pos.y + size + radius) {
		// If were above square center we return down
		if (center.y < pos.y + (size / 2)) return Vec2{0, 1};
		// else return up
		return Vec2{0, -1};
	}

	// Check corners
	Vec2 closest_point{0, 0};

	if (center.x < pos.x) closest_point.x = pos.x;
	else if (center.x > pos.x + size) closest_point.x = pos.x + size;

	if (center.y < pos.y) closest_point.y = pos.y;
	else if (center.y > pos.y + size) closest_point.y = pos.y + size;

	Vec2 collision_vector = closest_point - center;
	float distance_to_point = collision_vector.Mag();

	if (distance_to_point > radius) return {0,0};
	return NormalizeVec2(collision_vector);
}