#include "Camera.h"

#include <algorithm>

void Camera::Init() {
	distance_to_move_a_second = starting_speed;
}

void Camera::SetFollow(Vec2* position_to_follow) {
	followed_position = position_to_follow;
}

void Camera::Update(const float dt) {
	// Camera Follow
	if (followed_position) {
		const Vec2 vec_to_followed = (*followed_position) - position;
		const float distance_squared = vec_to_followed.MagSquared();


		const float distance_to_move = distance_to_move_a_second * dt;

		const float distance_to_move_squared = distance_to_move * distance_to_move;

		// If were closer than we would move, just set to the position.
		if (distance_squared < distance_to_move_squared) {
			position = *followed_position;
			distance_to_move_a_second = starting_speed;
			return;
		}

		distance_to_move_a_second += 50.0f * dt;
		distance_to_move_a_second = std::min(distance_to_move_a_second, max_speed);

		const Vec2 vec_to_followed_normalized = NormalizeVec2(vec_to_followed);
		const Vec2 move_vector = vec_to_followed_normalized * distance_to_move;

		position = position + move_vector;
	}
}