#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include <cassert>

#include "GameLevel.h"

void Enemy::Init(Game* game, const EnemyType enemy_type) {
	sprite.Init(10);
	for (int i = 0; i < 10; i++) {
		sprite.images[i] = &game->images[SwordsmanWalk1 + i];
	}
	sprite.animation_framerate = 15;

	attacking_sprite.Init(15);
	for (int i = 0; i < 15; i++) {
		attacking_sprite.images[i] = &game->images[SwordsmanAttack1 + i];
	}
	attacking_sprite.animation_framerate = 30; // Attack takes 0.5 seconds.
	attack_duration = 0.5f;

	dying_sprite.Init(1);
	dying_sprite.images[0] = &game->images[SwordsmanWalk1];

	type = enemy_type;
	max_health = ENEMY_HEALTH_BY_TYPE[type];
	attack_range = ENEMY_ATTACK_RANGE_BY_TYPE[type];
	current_health = max_health;

	collider.radius = 16.0f;

	state = Walking;
}

void Enemy::Update(const Game* game) {
	sprite.depth = static_cast<int>(-position.y);
	attacking_sprite.depth = static_cast<int>(-position.y);
	dying_sprite.depth = static_cast<int>(-position.y);

	switch (state) {
	case Walking:
		sprite.Update(game);
		Walk(game);
		break;
	case Attacking:
		attacking_sprite.Update(game);
		Attack(game);
		break;
	case Dying:
		dying_sprite.Update(game);
		Die(game);
		break;
	}

	collider.center = position;
}

void Enemy::Walk(const Game* game) {
	if (type == Turret) return; // Turret doesnt walk.

	Vec2 to_player_vector = player->position - position;

	float distance_from_player_squared = (to_player_vector).MagSquared();

	if (distance_from_player_squared <= (attack_range * attack_range)) {
		// Need to attack not move.
		SetState(Attacking);
	}
	else {
		const Vec2 move_direction = NormalizeVec2(to_player_vector);
		position = position + (move_direction * speed * game->game_time);

		if (move_direction.x < 0) {
			sprite.flip = true;
			attacking_sprite.flip = true;
			dying_sprite.flip = true;
		}
		else if (move_direction.x > 0) {
			sprite.flip = false;
			attacking_sprite.flip = false;
			dying_sprite.flip = false;
		}
	}
}

void Enemy::Attack(const Game* game) {
	if (current_attack_time >= attack_duration) {
		GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
		Player& player = level->player;

		Vec2 attack_direction = player.position - position;

		// Probably want to actually do the attack, ie shoot projectile, or check damage collisions.
		switch (type) {
		case Zombie:
			// Can just check range to player, if still in range, damage.
			break;
		case Archer:
			// Shoot a projectile towards the player.
			break;
		case Bomber:
			// Die and trigger explosion
			break;
		case Turret:
			// Shoot a projectile towards player.
			break;
		}
		SetState(Walking);
		return;
	}

	current_attack_time += game->game_time;
}

void Enemy::Die(const Game* game) {
	current_die_time += game->game_time;
	if (current_die_time >= die_duration) {
		// Need to remove this enemy from the level, Just call a level.QueueRemoveEnemy() function.
		// That will remove enemies end of frame.
		SetState(Dead);
	}
}

void Enemy::Draw(Game* game) {
	switch (state) {
	case Walking:
		game->DrawSprite(sprite, position);
		break;
	case Attacking:
		game->DrawSprite(attacking_sprite, position);
		break;
	case Dying:
		game->DrawSprite(dying_sprite, position);
		break;
	}
}

void Enemy::SetState(const State new_state) {
	if (state == new_state) return;

	state = new_state;

	// Make sure were playing the new animation from the start.
	switch (state) {
	case State::Walking:
		sprite.Reset();
		break;
	case State::Attacking:
		attacking_sprite.Reset();
		current_attack_time = 0;
		break;
	case State::Dying:
		dying_sprite.Reset();
		current_die_time = 0;
		break;
	}
}

void Enemy::Hit(float damage) {
	current_health -= damage;

	if (current_health <= 0) {
		SetState(Dying);
	}
}