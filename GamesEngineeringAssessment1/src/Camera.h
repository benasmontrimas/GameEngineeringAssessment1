#pragma once

#include "CustomBase.h"

// TODO: To lock the camera in to the map, can just add a simple boundary to check against. use ints and set min and max as INT_MAX/MIN.
class Camera {
public:
	Camera() = default;

	void Init();
	void SetFollow(Vec2* position_to_follow);
	void Update(float dt);

	Vec2 position {0, 0};
	Vec2* followed_position{nullptr};
	float distance_to_move_a_second;
	float max_speed = 150.0f;
	float starting_speed = 100.0f;
};