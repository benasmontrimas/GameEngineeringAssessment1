#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include <cassert>

void Enemy::Init(Game* game, const EnemyType enemy_type) {
	sprite.Init(1);

	type = enemy_type;
	max_health = ENEMY_HEALTH_BY_TYPE[type];
	attack_range = ENEMY_ATTACK_RANGE_BY_TYPE[type];
	current_health = max_health;
	sprite.images[0] = &game->images[ZombieBase + static_cast<unsigned int>(type)];

	collider.radius = 16.0f;
}

void Enemy::Update(const Game* game) {
	sprite.depth = static_cast<int>(-position.y);

	// AI
	switch (type) {
	case Zombie: // All but turret have same ai, walk until your in attack range, and attack, turret just doesnt move.
	case Bomber:
	case Archer:
		{
			Vec2 to_player_vector = player->position - position;

			float distance_from_player_squared = (to_player_vector).MagSquared();

			if (distance_from_player_squared <= (attack_range * attack_range)) {
				// Need to attack not move.
			}
			else {
				const Vec2 move_direction = NormalizeVec2(to_player_vector);
				position = position + (move_direction * speed * game->game_time);

				if (move_direction.x < 0) sprite.flip = true;
				else if (move_direction.x > 0) sprite.flip = false;
			}
		}
		break;
	case Turret:
		// No Movement

		break;
	default:
		assert("Enemy Type not implemented.");
	}

	collider.center = position;
}

void Enemy::Draw(Game* game) {
	game->DrawSprite(sprite, position);
}