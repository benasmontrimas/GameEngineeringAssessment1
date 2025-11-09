#include "Vec2.h"

Vec2 NormalizeVec2(const Vec2& vec) {
	 const float magnitude = vec.Mag();
	 return vec / magnitude;
}

float Dot(const Vec2& a, const Vec2& b) {
	return (a.x * b.x) + (a.y * b.y);
}