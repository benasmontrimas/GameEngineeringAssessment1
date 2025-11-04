#include "Player.h"
#include "Game.h"

void Player::Init(Game* game) {
	sprite.Init(1);
	sprite.images[0] = &game->images[PlayerWalk2];

	walking_sprite.Init(2);
	walking_sprite.images[0] = &game->images[PlayerWalk2];
	walking_sprite.images[1] = &game->images[PlayerWalk3];
	walking_sprite.animation_framerate = 4;

	dying_sprite.Init(1);
	dying_sprite.images[0] = &game->images[PlayerWalk2];

	// Need to make the dying sprite. Don't have any images for it yet.

	collider.radius = 32.0f;
}

void Player::Update(Game* game) {
	switch (state) {
	case State::Idle:
		sprite.Update(game);
		sprite.depth = -position.y;
		HandleInput(game);
		break;
	case State::Walking:
		walking_sprite.Update(game);
		walking_sprite.depth = -position.y;
		HandleInput(game);
		break;
	case State::Dying:
		break;
	}

	collider.center = position;
}

void Player::Draw(Game* game) const
{
	switch (state) {
	case State::Idle:
		game->DrawSprite(sprite, position);
		break;
	case State::Walking:
		game->DrawSprite(walking_sprite, position);
		break;
	case State::Dying:
		game->DrawSprite(dying_sprite, position);
	}
}

void Player::HandleInput(const Game* game) {
	const GamesEngineeringBase::Window& window = game->window;

	movement_direction = { 0, 0 };
	if (window.keyPressed('W') or window.keyPressed(VK_UP)) movement_direction.y -= 1.0f;
	if (window.keyPressed('S') or window.keyPressed(VK_DOWN)) movement_direction.y += 1.0f;
	if (window.keyPressed('A') or window.keyPressed(VK_LEFT)) movement_direction.x -= 1.0f;
	if (window.keyPressed('D') or window.keyPressed(VK_RIGHT)) movement_direction.x += 1.0f;

	if (window.keyPressed(VK_SPACE)); // Attack with active weapons.

	if (movement_direction != Vec2{ 0, 0 }) {
		const Vec2 movement_vector = NormalizeVec2(movement_direction) * movement_speed * game->game_time;
		position = position + movement_vector;
		SetState(State::Walking);
	}
	else {
		SetState(State::Idle);
	}

	if (movement_direction.x * game->game_time < 0) {
		sprite.flip = true;
		walking_sprite.flip = true;
	}
	else if (movement_direction.x * game->game_time > 0) {
		sprite.flip = false;
		walking_sprite.flip = false;
	}
}

void Player::SetState(const State new_state) {
	if (state == new_state) return;

	state = new_state;

	// Make sure were playing the new animation from the start.
	switch (state) {
	case State::Idle:
		sprite.Reset();
		break;
	case State::Walking:
		walking_sprite.Reset();
		break;
	case State::Dying:
		dying_sprite.Reset();
		break;
	}
}