#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include <cassert>

#include "GameLevel.h"

void Enemy::Init(Game* game, const EnemyType enemy_type) {
	constexpr GameImages walking_animation_offset_by_type[ENEMY_TYPE_COUNT] = {
		SwordsmanWalk1,
		ArcherWalk1,
		BomberWalk1,
		TurretIdle1
	};
	constexpr int walking_animation_frames_by_type[ENEMY_TYPE_COUNT] = {10, 10, 10, 3};
	sprite.Init(walking_animation_frames_by_type[enemy_type]);
	for (int i = 0; i < walking_animation_frames_by_type[enemy_type]; i++) {
		sprite.images[i] = &game->images[walking_animation_offset_by_type[enemy_type] + i];
	}
	sprite.animation_framerate = float(walking_animation_frames_by_type[enemy_type]) * 1.5f;

	constexpr GameImages attacking_animation_offset_by_type[ENEMY_TYPE_COUNT] = {
		SwordsmanAttack1,
		ArcherAttack1,
		BomberAttack1,
		TurretAttack1
	};
	constexpr int attacking_animation_frames_by_type[ENEMY_TYPE_COUNT] = {15, 8, 6, 5};
	attacking_sprite.Init(attacking_animation_frames_by_type[enemy_type]);
	for (int i = 0; i < attacking_animation_frames_by_type[enemy_type]; i++) {
		attacking_sprite.images[i] = &game->images[attacking_animation_offset_by_type[enemy_type] + i];
	}
	// All attacks take 1 second.
	attacking_sprite.animation_framerate = float(attacking_animation_frames_by_type[enemy_type]);
	attack_duration = float(attacking_animation_frames_by_type[enemy_type]) / attacking_sprite.animation_framerate;

	constexpr GameImages dying_animation_offset_by_type[ENEMY_TYPE_COUNT] = {
		SwordsmanDeath1,
		ArcherDeath1,
		BomberDeath1,
		TurretDeath1
	};
	constexpr int dying_animation_frames_by_type[ENEMY_TYPE_COUNT] = {10, 12, 12, 8};
	dying_sprite.Init(dying_animation_frames_by_type[enemy_type]);
	for (int i = 0; i < dying_animation_frames_by_type[enemy_type]; i++) {
		dying_sprite.images[i] = &game->images[dying_animation_offset_by_type[enemy_type] + i];
	}
	// All death animations take 0.5 seconds.
	dying_sprite.animation_framerate = float(dying_animation_frames_by_type[enemy_type]) * 2.0f;
	die_duration = float(dying_animation_frames_by_type[enemy_type]) / dying_sprite.animation_framerate;

	type = enemy_type;
	max_health = ENEMY_HEALTH_BY_TYPE[type];
	attack_range = ENEMY_ATTACK_RANGE_BY_TYPE[type];
	current_health = max_health;

	collider.radius = 16.0f;

	state = Walking;
}

void Enemy::Deinit() {
	sprite.Deinit();
	attacking_sprite.Deinit();
	dying_sprite.Deinit();
}

void Enemy::Update(Game* game) {
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
		current_die_time += game->game_time;
		if (current_die_time >= die_duration) {
				Die(game);
		}
		break;
	}

	for (int i = projectile_count - 1; i >= 0; i--) {
		projectiles[i].Update(game);

		if (projectiles[i].is_dead) {
			projectiles[i].sprite.Deinit(); // Need to delete the sprite images array otherwise we leak it.
			projectiles[i] = projectiles[projectile_count - 1];
			projectile_count--;
			continue;
		}

		// Check collision here
	}

	collider.center = position + Vec2{24, 24};
}

void Enemy::Walk(Game* game) {
	Vec2 to_player_vector = player->position - position;

	float distance_from_player_squared = (to_player_vector).MagSquared();

	if (distance_from_player_squared <= (attack_range * attack_range)) {
		// Need to attack not move.
		SetState(Attacking);
	}

	const Vec2 move_direction = NormalizeVec2(to_player_vector);
	if (type != Turret)	position = position + (move_direction * speed * game->game_time);

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

void Enemy::Attack(Game* game) {
	current_attack_time += game->game_time;

	if (current_attack_time >= attack_duration) {
		GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
		Player& player = level->player;

		Vec2 attack_direction = NormalizeVec2(player.position - position);

		// Probably want to actually do the attack, ie shoot projectile, or check damage collisions.
		switch (type) {
		case Swordsman:
			// Can just check range to player, if still in range, damage.
			break;
		case Bomber:
			// Die and trigger explosion
			break;
		case Archer:
		case Turret:
			// Shoot a projectile towards the player.
			if (projectile_count >= max_projectile_count) return;

			projectiles[projectile_count].Init(game);

			projectiles[projectile_count].sprite.Init(4);
			for (int animation_index = 0; animation_index < 4; animation_index++) {
				projectiles[projectile_count].sprite.images[animation_index] = &game->images[ArcherArrow1 + animation_index];
			}
			projectiles[projectile_count].sprite.animation_framerate = 16;

			projectiles[projectile_count].Shoot(game, position, attack_direction, 400, 3.0f);
			projectile_count++;
			break;
		}
		SetState(Walking);
		return;
	}
}

void Enemy::Die(Game* game) {
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

	for (int i = 0; i < projectile_count; i++) {
		projectiles[i].Draw(game);
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