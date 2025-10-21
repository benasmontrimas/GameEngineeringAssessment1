#include "Player.h"
#include "Game.h"

void Player::Init(Game* game) {
	sprite.Init(1);
	sprite.images[0] = &game->images[PlayerBase];

	collider.radius = 16.0f;
}

void Player::Update(Game* game) {
	sprite.depth = -position.y;

	GamesEngineeringBase::Window& window = game->window;

	movement_direction = { 0, 0 };
	if (window.keyPressed('W') or window.keyPressed(VK_UP)) movement_direction.y -= 1.0f;
	if (window.keyPressed('S') or window.keyPressed(VK_DOWN)) movement_direction.y += 1.0f;
	if (window.keyPressed('A') or window.keyPressed(VK_LEFT)) movement_direction.x -= 1.0f;
	if (window.keyPressed('D') or window.keyPressed(VK_RIGHT)) movement_direction.x += 1.0f;

	if (movement_direction != Vec2{0, 0}) {
		Vec2 movement_vector = NormalizeVec2(movement_direction) * movement_speed * game->delta_time;
		position = position + movement_vector;
	}

	collider.center = position;
}