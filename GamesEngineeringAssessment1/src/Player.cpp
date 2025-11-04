#include "Player.h"
#include "Sword.h"
#include "Game.h"

void Player::Init(Game* game) {
	sprite.Init(7);
	for (int i = 0; i < 7; i++) {
		sprite.images[i] = &game->images[PlayerIdle1 + i];
	}
	sprite.animation_framerate = 10;

	walking_sprite.Init(9);
	for (int i = 0; i < 9; i++) {
		walking_sprite.images[i] = &game->images[PlayerWalk1 + i];
	}
	walking_sprite.animation_framerate = 10;

	dying_sprite.Init(1);
	dying_sprite.images[0] = &game->images[PlayerWalk2];

	// Need to make the dying sprite. Don't have any images for it yet.

	collider.radius = 32.0f;

	 weapons[0] = new Sword;
	 weapons[0]->Init(game);
}

void Player::Update(Game* game) {
	if (state != State::Dying) { // already have a switch but i dont want repeat this loop.
		for (int i = 0; i < 2; i++) {
			if (weapons[i]) weapons[i]->Update(game);
		}
	}

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
	if (state != State::Dying) { // already have a switch but i dont want repeat this loop.
		for (int i = 0; i < 2; i++) {
			if (weapons[i]) weapons[i]->Draw(game);
		}
	}

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