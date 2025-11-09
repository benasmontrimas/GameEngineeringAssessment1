#include "Camera.h"

void Camera::Init() {
	distance_to_move_a_second = starting_speed;
}

void Camera::SetFollow(Vec2* position_to_follow) {
	followed_position = position_to_follow;
}

void Camera::Update(const float dt) {
	// Camera Follow
	if (followed_position) {
		FollowPosition(dt);
	}

	// If capped to boundary we just clamp the values of position to the min and max pos.
	if (capped_to_boundary) {
		position.x = std::max(position.x, min_position.x);
		position.x = std::min(position.x, max_position.x);

		position.y = std::max(position.y, min_position.y);
		position.y = std::min(position.y, max_position.y);
	}
}

void Camera::FollowPosition(const float dt) {
	const Vec2 vec_to_followed = (*followed_position) - position;
	const float distance_squared = vec_to_followed.MagSquared();

	// If already at the position we want to follow we don't need to do anything.
	if (distance_squared <= 0.001f) return;

	const float distance_to_move = distance_to_move_a_second * dt;

	const float distance_to_move_squared = distance_to_move * distance_to_move;

	// If were closer than we would move, just set to the position.
	if (distance_squared < distance_to_move_squared) {
		position = *followed_position;
		distance_to_move_a_second = starting_speed;
		return;
	}

	// Add acceleration to the camera, so it gets faster the longer it's in flight.
	distance_to_move_a_second += 50.0f * dt;
	distance_to_move_a_second = std::min(distance_to_move_a_second, max_speed);

	const Vec2 vec_to_followed_normalized = NormalizeVec2(vec_to_followed);
	const Vec2 move_vector = vec_to_followed_normalized * distance_to_move;

	position = position + move_vector;
}