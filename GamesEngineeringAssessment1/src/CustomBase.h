#pragma once

#include <iostream>

struct Vec2 {
	float x, y;

	float& operator[](int i) {
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

	bool operator!=(Vec2 other) {
		return (x != other.x or y != other.y);
	}

	float MagSquared() const {
		return (x * x) + (y * y);
	}

	float Mag() const {
		return sqrt(MagSquared());
	}
};

Vec2 NormalizeVec2(const Vec2& vec);


// TODO: Is this useful? Ideally i replace all my c arrays with this. Need to test if it works the same way.
// An fixed size array which keeps track of how many items are in it.
// Does not do any checks for access and removal.
template <typename T>
class List {
	T* data;
	unsigned int size;

	List(unsigned int size) : size(size) { data = (T*)malloc(sizeof(T) * size); }
	List(List<T>& other) = delete;
	List(List<T>&& other) = delete;
	~List() { delete[] data; };

	// Adds an empty element and returns a ref to it. Does not initialize the value.
	T& append() {
		size++;
		return data[size - 1];
	}

	void Append(const T& val) {
		data[size] = val;
		size++;
	}

	// Remove an element and keep order of existing elements.
	void Remove(unsigned int index) {
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
	void operator=(List<T> other) = delete;
};