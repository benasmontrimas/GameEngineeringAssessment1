#include "Collider.h"

bool Collider::CheckCollision(Collider other) {
	Vec2 distance_vector = other.center - center;
	float distance_squared = distance_vector.MagSquared();
	return distance_squared < ((other.radius * other.radius) + (radius * radius));
}