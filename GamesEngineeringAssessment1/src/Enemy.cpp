#include "Enemy.h"
#include "Game.h"
#include <cassert>

void Enemy::Init(Game* game, EnemyType _type) {

	type = _type;
	max_health = ENEMY_HEALTH_BY_TYPE[type];
	current_health = max_health;
	sprite.image = &game->images[ZombieBase + type];
}

void Enemy::Update(Game* game) {
	sprite.depth = -position.y;

	// AI
	switch (type) {
	case Zombie: // Zombie and tank have the same AI.
	case Tank:
		Vec2 move_direction = NormalizeVec2(game->player.position - position);
		position = position + (move_direction * speed * game->delta_time);
		break;
	case Archer:
		break;
	default:
		assert("Enemy Type not implemented.");
	}
}