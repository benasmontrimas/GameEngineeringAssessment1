#pragma once

#include <iostream>

using uint = unsigned int;

struct Vec2 {
	float x, y;

	float& operator[](const int i) {
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		default:
			std::cout << "Trying to access index which is out of bounds.";
			break;
		}
		throw std::exception("Out of bounds Vec2 access.");
	}

	Vec2 operator+(const Vec2& other) const {
		return Vec2{x + other.x, y + other.y };
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2{x - other.x, y - other.y };
	}

	Vec2 operator*(const float val) const {
		return Vec2{x * val, y * val };
	}

	Vec2 operator*(const Vec2& o) const {
		return Vec2{x * o.x, y * o.y};
	}

	Vec2 operator/(const float val) const {
		return Vec2{x / val, y / val };
	}

	bool operator==(const Vec2& other) const
	{
		return (x == other.x and y == other.y);
	}

	bool operator!=(const Vec2& other) const
	{
		return !(*this == other);
	}

	[[nodiscard]] float MagSquared() const {
		return (x * x) + (y * y);
	}

	[[nodiscard]] float Mag() const {
		return sqrt(MagSquared());
	}
};

Vec2 NormalizeVec2(const Vec2& vec);

float Dot(const Vec2& a, const Vec2& b);



// Was going to use this for my pools, but i haven't tested this or put too much effort, should have
// Made this from the start. Would have reduced manually keeping track of size.
template <typename T, int N>
class Pool {
public:
	T data[N];
	unsigned int size{0};

	Pool() = default;
	~Pool() = default;
	Pool(Pool<T, N>& other) = delete;
	Pool(Pool<T, N>&& other) = delete;
	Pool<T, N>& operator=(Pool<T, N> other) = delete;
	Pool<T, N>& operator=(Pool<T, N>& other) = delete;
	Pool<T, N>& operator=(Pool<T, N>&& other) = delete;

	// Adds an empty element and returns a ref to it. Does not initialize the value.
	T& Append() {
		size++;
		return data[size - 1];
	}

	void Append(const T& val) {
		data[size] = val;
		size++;
	}

	// Remove an element and keep order of existing elements.
	void Remove(const unsigned int index) {
		for (unsigned int i = index; i < size - 1; i++) {
			data[i] = data[i + 1];
		}
		size--;
	}
	// Remove an element with no guarantee order stays the same.
	void UnorderedRemove(unsigned int index) {
		data[index] = data[size - 1];
		size--;
	}
	// remove the last element and return it. (returns a copy).
	T Pop() {
		size--;
		return data[size];
	}

	T& operator[](unsigned int index) {
		return data[index];
	}
};