#include "CustomBase.h"

Vec2 NormalizeVec2(const Vec2& vec) {
	 const float magnitude = vec.Mag();
	 return vec / magnitude;
}