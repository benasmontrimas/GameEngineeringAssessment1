#pragma once

#include "Vec2.h"

class Camera {
public:
	Camera() = default;

	void Init();
	void SetFollow(Vec2* position_to_follow);
	void Update(float dt);

	void FollowPosition(const float dt);

	Vec2 position {.x = 0, .y = 0};
	Vec2* followed_position{nullptr};
	float distance_to_move_a_second;
	float max_speed = 150.0f;
	float starting_speed = 100.0f;

	bool capped_to_boundary;
	Vec2 min_position;
	Vec2 max_position;
};