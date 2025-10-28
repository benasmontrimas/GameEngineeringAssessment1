#include "Collider.h"

bool Collider::CheckCollision(const Collider& other) const
{
	const Vec2 distance_vector = other.center - center;
	const float distance_squared = distance_vector.MagSquared();
	return distance_squared < ((other.radius * other.radius) + (radius * radius));
}
