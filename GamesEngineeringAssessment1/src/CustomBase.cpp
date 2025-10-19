#include "CustomBase.h"

Vec2 NormalizeVec2(const Vec2& vec) {
	 float magnitude = vec.Mag();
	 return vec / magnitude;
}

