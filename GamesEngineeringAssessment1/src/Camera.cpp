#include "Camera.h"

void Camera::Init() {
	distance_to_move_a_second = starting_speed;
}

void Camera::SetFollow(Vec2* position_to_follow) {
	followed_position = position_to_follow;
}

void Camera::Update(float dt) {
	// Camera Follow
	if (followed_position) {
		Vec2 vec_to_followed = (*followed_position) - position;
		float distance_squared = vec_to_followed.MagSquared();


		float distance_to_move = distance_to_move_a_second * dt;

		float distance_to_move_squared = distance_to_move * distance_to_move;
		// If were closer than we would move, just set to the position.
		if (distance_squared < distance_to_move_squared) {
			position = *followed_position;
			distance_to_move_a_second = starting_speed;
			return;
		}

		distance_to_move_a_second += 50.0f * dt;
		if (distance_to_move_a_second > max_speed) distance_to_move_a_second = max_speed;

		Vec2 vec_to_followed_normalized = NormalizeVec2(vec_to_followed);
		Vec2 move_vector = vec_to_followed_normalized * distance_to_move;

		position = position + move_vector;
	}
}