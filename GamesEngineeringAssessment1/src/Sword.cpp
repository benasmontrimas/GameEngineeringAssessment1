#include "Sword.h"
#include "Game.h"
#include "Enemy.h"
#include "GameLevel.h"

void Sword::Init(Game* game) {
	type = WeaponType::AutoAttack;

	name = "Sword";
	description = "A fast swinging auto attack.";

	cooldown = 1.0f;
	damage = 100.0f;

	// Each projectiles gets a copy of this, dont need to worry about keeping a ref to it.
	Sprite projectile_sprite;
	projectile_sprite.Init(8);
	for (int animation_index = 0; animation_index < 8; animation_index++) {
		projectile_sprite.images[animation_index] = &game->images[SwordThrow1 + animation_index];
	}

	for (int i = 0; i < 100; i++) {
		projectiles[i].Init(game, projectile_sprite);
	}

	// Need to set sprites, don't have any yet;
	//attack_animation
	//sprite
	//icon
}

void Sword::Attack(Game* game) {
	if (projectile_count >= 100) return;
	// Make it just spawn a sword projectile.
	// No attack duration.
	GameLevel* level = (GameLevel*)game->GetLevel();
	Player& player = level->player;

	GetAttackDirection(game);
	projectiles[projectile_count].Shoot(game, player.position, attack_direction_, 100, 3.0f);
	projectile_count++;
}

void Sword::Update(Game* game) {
	last_attack += game->game_time;
	if (last_attack >= cooldown) {
		Attack(game);
		last_attack -= cooldown;
	}


	GameLevel* level = (GameLevel*)game->GetLevel();
	Enemy* enemies = level->enemies;
	int enemies_alive = level->enemies_alive;

	// We go backwards so that we can remove projectiles if they collide.
	for (int i = projectile_count - 1; i >= 0; i--) {
		projectiles[i].Update(game);

		if (projectiles[i].is_dead) {
			projectiles[i] = projectiles[projectile_count - 1];
			projectile_count--;
			continue;
		}

		// Do collisions here so we only check the ones we need... If we do in projectile class, we dont know
		// What we want to collide with and dont want to check all enemies if we only want projectiles to collide with player.

		for (int enemy_i = 0; enemy_i < enemies_alive; enemy_i++) {
			if (projectiles[i].collider.CheckCollision(enemies[enemy_i].collider)) {
				enemies[enemy_i].Hit(damage);

				projectiles[i] = projectiles[projectile_count - 1];
				projectile_count--;
				break;
			}
		}
	}
}

void Sword::Draw(Game* game) {
	for (int i = 0; i < projectile_count; i++) {
		projectiles[i].Draw(game);
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
		const float distance_squared = (enemies[i].position - player.position).MagSquared();
		if (distance_squared >= closest_distance_squared) continue;

		closest_enemy_index = i;
		closest_distance_squared = distance_squared;
	}

	attack_direction_ = NormalizeVec2(enemies[closest_enemy_index].position - player.position);
}
