#pragma once

#include <iostream>

struct Vec2 {
	float x, y;

	float operator[](int i) {
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		default:
			std::cout << "Trying to access index which is out of bounds.";
			break;
		}
	}

	Vec2 operator+(Vec2 other) const {
		return Vec2{ x + other.x, y + other.y };
	}

	Vec2 operator-(Vec2 other) const {
		return Vec2{ x - other.x, y - other.y };
	}

	Vec2 operator*(float val) const {
		return Vec2{ x * val, y * val };
	}

	Vec2 operator/(float val) const {
		return Vec2{ x / val, y / val };
	}

	float MagSquared() const {
		return (x * x) + (y * y);
	}

	float Mag() const {
		return sqrt(MagSquared());
	}
};

Vec2 NormalizeVec2(const Vec2& vec);

