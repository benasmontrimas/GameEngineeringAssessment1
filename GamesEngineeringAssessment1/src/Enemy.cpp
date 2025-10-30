#include "Enemy.h"
#include "Game.h"
#include <cassert>

void Enemy::Init(Game* game, const EnemyType enemy_type) {
	sprite.Init(1);

	type = enemy_type;
	max_health = ENEMY_HEALTH_BY_TYPE[type];
	current_health = max_health;
	sprite.images[0] = &game->images[ZombieBase + static_cast<unsigned int>(type)];

	collider.radius = 16.0f;
}

void Enemy::Update(const Game* game) {
	sprite.depth = static_cast<int>(-position.y);

	// AI
	switch (type) {
	case Zombie: // Zombie and tank have the same AI.
	case Tank:
		{
			// NOTE: THIS IS WRONG, NEED TO SPECIFY THE GOTO POSITION OF ENEMIES, CURRENTLY GO TO CAMERA (WRONG)
			const Vec2 move_direction = NormalizeVec2(game->camera.position - position);
			position = position + (move_direction * speed * game->delta_time);
			
			if (move_direction.x < 0) sprite.flip = true;
			else if (move_direction.x > 0) sprite.flip = false;
		}
		break;
	case Archer:
		break;
	default:
		assert("Enemy Type not implemented.");
	}

	collider.center = position;
}