#include "Collider.h"

// Circle on circle collision.
// Used for everything other than player-tile collision checks.
bool Collider::CheckCollision(const Collider& other) const
{
	const Vec2 distance_vector = other.center - center;
	const float distance_squared = distance_vector.MagSquared();
	return distance_squared < ((other.radius * other.radius) + (radius * radius));
}

// Check rect to sphere collision. I'm sure there's a more elegant way to do this.
// Used for player-tile collision.
Vec2 Collider::CheckCollisionSquare(const Vec2& pos, const float size) const
{
	// Check rect dimensions + radius
	if (center.x + radius < pos.x) return {.x = 0, .y = 0};
	if (center.x - radius > pos.x + size) return {.x = 0, .y = 0};
	if (center.y + radius < pos.y) return {.x = 0, .y = 0};
	if (center.y - radius > pos.y + size) return {.x = 0, .y = 0};

	// check collider center to the 2 rects made if we extend them by the radius (this checks everything but corners.
	if (center.x > pos.x - radius and
		center.x < pos.x + size + radius and
		center.y > pos.y and
		center.y < pos.y + size) {
		// If were to teh left of square center we return right
		if (center.x < pos.x + (size / 2)) return Vec2{.x = 1, .y = 0};
		// else return left
		return Vec2{.x = -1, .y = 0};
	}

	if (center.x > pos.x and
		center.x < pos.x + size and
		center.y > pos.y - radius and
		center.y < pos.y + size + radius) {
		// If were above square center we return down
		if (center.y < pos.y + (size / 2)) return Vec2{.x = 0, .y = 1};
		// else return up
		return Vec2{.x = 0, .y = -1};
	}

	// Check corners
	Vec2 closest_point{.x = 0, .y = 0};

	if (center.x < (pos.x + (size / 2))) closest_point.x = pos.x;
	else closest_point.x = pos.x + size;

	if (center.y < (pos.y + (size / 2))) closest_point.y = pos.y;
	else closest_point.y = pos.y + size;

	const Vec2 collision_vector = closest_point - center;
	const float distance_to_point = collision_vector.Mag();

	if (distance_to_point >= radius) return {.x = 0, .y = 0};

	return NormalizeVec2(collision_vector);
}