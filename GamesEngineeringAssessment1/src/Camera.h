#pragma once

#include "CustomBase.h"

class Camera {
public:
	Camera() = default;

	void Init();
	void SetFollow(Vec2* position_to_follow);
	void Update(float dt);

	Vec2 position;
	Vec2* followed_position;
	float distance_to_move_a_second;
	float max_speed = 150.0f;
	float starting_speed = 100.0f;
};