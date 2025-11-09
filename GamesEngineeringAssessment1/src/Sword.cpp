#include "Sword.h"
#include "Game.h"
#include "Enemy.h"
#include "GameLevel.h"

void Sword::Init(Game* game) {
	cooldown = 0.5f;
	damage = 100.0f;
}

void Sword::Update(Game* game) {
	GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
	const Player& player = level->player;
	Enemy* enemies = level->enemies;
	const int enemies_alive = static_cast<int>(level->enemies_alive);

	last_attack += game->game_time;
	if (last_attack >= cooldown) {
		Attack(game);
		// If power up active we just reduce cooldown by half
		if (player.power_up_time_left > 0.0f) {
			last_attack -= cooldown / 2;
		} else {
			last_attack -= cooldown;
		}
	}

	// We go backwards so that we can remove projectiles if they collide.
	for (int i = projectile_count_ - 1; i >= 0; i--) {
		projectiles_[i].Update(game);

		if (projectiles_[i].IsDead()) {
			projectiles_[i] = projectiles_[projectile_count_ - 1];
			projectile_count_--;
			continue;
		}

		// Do collisions here so we only check the ones we need... If we do in projectile class, we dont know
		// What we want to collide with and don't want to check all enemies if we only want projectiles to collide with player.

		for (int enemy_i = 0; enemy_i < enemies_alive; enemy_i++) {
			// Ignore Dying enemies. do not want to target them.
			if (enemies[enemy_i].state == Enemy::State::Dying) continue;

			if (projectiles_[i].collider.CheckCollision(enemies[enemy_i].collider)) {
				enemies[enemy_i].Hit(damage);

				projectiles_[i] = projectiles_[projectile_count_ - 1];
				projectile_count_--;
				break;
			}
		}
	}
}

void Sword::Draw(Game* game) {
	for (int i = 0; i < projectile_count_; i++) {
		projectiles_[i].Draw(game);
	}
}

void Sword::GetAttackDirection(const Game* game)
{
	// Sword is only active in game level, so this should be ok.
	const GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
	const Player& player = level->player;
	const Enemy* enemies = level->enemies;
	const unsigned int enemy_count = level->enemies_alive;

	if (enemy_count == 0) return;

	unsigned int closest_enemy_index = 0;
	float closest_distance_squared = (enemies[0].position - player.position).MagSquared();
	for (unsigned int i = 1; i < enemy_count; i++)
	{
		// Ignore Dying enemies. do not want to target them.
		if (enemies[i].state == Enemy::State::Dying) continue;
		const float distance_squared = (enemies[i].position - player.position).MagSquared();
		if (distance_squared >= closest_distance_squared) continue;

		closest_enemy_index = i;
		closest_distance_squared = distance_squared;
	}

	attack_direction_ = NormalizeVec2(enemies[closest_enemy_index].position - player.position);
}

void Sword::Attack(Game* game) {
	if (projectile_count_ >= max_projectile_count) return;
	// Make it just spawn a sword projectile.
	// No attack duration.
	const GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
	const Player& player = level->player;

	projectiles_[projectile_count_].Init(game);

	projectiles_[projectile_count_].sprite.Init(8);
	for (int animation_index = 0; animation_index < 8; animation_index++) {
		projectiles_[projectile_count_].sprite.images[animation_index] = static_cast<GameImages>(SwordThrow1 + animation_index);
	}
	projectiles_[projectile_count_].sprite.animation_frame_rate = 16;

	GetAttackDirection(game);
	projectiles_[projectile_count_].Shoot(game, player.position, attack_direction_, 400, 2.0f);
	projectile_count_++;
}