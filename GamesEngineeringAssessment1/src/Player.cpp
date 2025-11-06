#include "Player.h"
#include "Sword.h"
#include "Game.h"
#include "GameLevel.h"

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

	dying_sprite.Init(18);
	for (int i = 0; i < 18; i++) {
		dying_sprite.images[0] = &game->images[PlayerDeath1 + i];
	}
	dying_sprite.animation_framerate = 18; // Play over 1 second

	// Need to make the dying sprite. Don't have any images for it yet.

	collider.radius = 14.0f;

	weapons[0] = new Sword;
	weapons[0]->Init(game);

	current_health = max_health;
	time_dying = 0;
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
		dying_sprite.Update(game);
		time_dying += game->game_time;
		if (time_dying > 1.0f) {
			state = State::Dead;
		}
		break;
	}

	collider.center = position + Vec2{24, 48}; // Offset collider to be slightly lower than center.

	// Check collisions with all enemies
	GameLevel* level = (GameLevel*)game->GetLevel();
	Enemy* enemies = level->enemies;
	unsigned int enemies_alive = level->enemies_alive;

	// Check collision cooldown, dont want player taking collision damage every frame...
	time_since_last_collision += game->game_time;
	if (time_since_last_collision >= collision_cooldown) {
		// Just to prevent the game going too long and the time since last overflowing or something
		time_since_last_collision = collision_cooldown;
		for (unsigned int i = 0; i < enemies_alive; i++) {
			// I think if enemy is dead we remove before this? hopefully.
			if (enemies[i].state == Enemy::State::Dying) continue;

			if (collider.CheckCollision(enemies[i].collider)) {
				Hit(10); // Take flat damage from colliding, enemies also attack so...
				time_since_last_collision = 0.0f;
				break; // Dont need to check rest, as we set collision cooldown.
			}
		}
	}

	// Check collision with tiles
	// If we collide along the x we just set movemnt direciton to 0. and so on.
	// This is more complicated than it has to be because our tiles dont actually have positions... just the map does.
	// Need to do the same calculation we use when we draw for this. This could be made a lot easier.

	if (movement_direction != Vec2{ 0, 0 }) {
		// Only need to check collisions if were moving.
		const TileMap& level_map = level->level_map;
		const CollisionData* collision_data = &level_map.collision_data[0];
		int collision_data_count = level_map.collision_data_count;
		float tile_size = static_cast<float>(level_map.tile_size);

		bool collided = false;
		Vec2 deciding_collision{0, 0};
		for (int i = 0; i < collision_data_count; i++) {
			Vec2 pos = collision_data[i].position;

			Vec2 collision_direction = collider.CheckCollisionSquare(pos, tile_size);
			if (collision_direction == Vec2{0, 0}) continue;
			collided = true;
			Vec2 perpendiculat_to_collision = Vec2{-collision_direction.y, collision_direction.x};

			std::cout << collision_direction.x << ":" << collision_direction.y << "\n";

			if (abs(deciding_collision.x) < abs(collision_direction.x)) {
				deciding_collision.x = collision_direction.x;
			}
			if (abs(deciding_collision.y) < abs(collision_direction.y)) {
				deciding_collision.y = collision_direction.y;
			}
		}

		if (collided) {
			if (deciding_collision.x < 0) {
				movement_direction.x = max(movement_direction.x, 0);
			} else if (deciding_collision.x > 0) {
				movement_direction.x = min(movement_direction.x, 0);
			}

			if (deciding_collision.y < 0) {
				movement_direction.y = max(movement_direction.y, 0);
			} else if (deciding_collision.y > 0) {
				movement_direction.y = min(movement_direction.y, 0);
			}

			position = last_position; // Move back to where we were before the collision.
			collider.center = position;
		}

		if (movement_direction == Vec2{ 0, 0 }) return;

		Vec2 movement_vector = NormalizeVec2(movement_direction) * movement_speed * game->game_time;
		last_position = position;
		position = position + movement_vector;
		SetState(State::Walking);
	}
	else {
		SetState(State::Idle);
	}
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

void Player::Hit(float damage) {
	current_health -= damage;
	if (current_health < 0) {
		SetState(State::Dying);
	}
}