#include "Enemy.h"
#include "Game.h"
#include "Player.h"

#include "GameLevel.h"

void Enemy::Init(Game* game, const EnemyType enemy_type) {

	type = enemy_type;
	max_health = enemy_health_by_type[type];
	attack_range = enemy_attack_range_by_type[type];
	current_health = max_health;
	speed = enemy_speed_by_type[type];
	damage = enemy_damage_by_type[type];
	collider.radius = 16.0f;

	constexpr GameImages walking_animation_offset_by_type[ENEMY_TYPE_COUNT] = {
		SwordsmanWalk1,
		ArcherWalk1,
		BomberWalk1,
		TurretIdle1
	};
	constexpr int walking_animation_frames_by_type[ENEMY_TYPE_COUNT] = {10, 10, 10, 3};

	sprite.Init(walking_animation_frames_by_type[enemy_type]);
	for (int i = 0; i < walking_animation_frames_by_type[enemy_type]; i++) {
		sprite.images[i] = static_cast<GameImages>(walking_animation_offset_by_type[enemy_type] + i);
	}
	sprite.animation_frame_rate = static_cast<float>(walking_animation_frames_by_type[enemy_type]) * 1.5f;

	constexpr GameImages attacking_animation_offset_by_type[ENEMY_TYPE_COUNT] = {
		SwordsmanAttack1,
		ArcherAttack1,
		BomberAttack1,
		TurretAttack1
	};
	constexpr int attacking_animation_frames_by_type[ENEMY_TYPE_COUNT] = {8, 8, 6, 5};

	attacking_sprite.Init(attacking_animation_frames_by_type[enemy_type]);
	for (int i = 0; i < attacking_animation_frames_by_type[enemy_type]; i++) {
		attacking_sprite.images[i] = static_cast<GameImages>(attacking_animation_offset_by_type[enemy_type] + i);
	}
	// All attacks take 1 second.
	attacking_sprite.animation_frame_rate = static_cast<float>(attacking_animation_frames_by_type[enemy_type]);
	// Except bomber. attack is 0.33
	if (type == Bomber) {
		attacking_sprite.animation_frame_rate *= 3.0f;
	}
	attack_duration = static_cast<float>(attacking_animation_frames_by_type[enemy_type]) / attacking_sprite.animation_frame_rate;

	constexpr GameImages dying_animation_offset_by_type[ENEMY_TYPE_COUNT] = {
		SwordsmanDeath1,
		ArcherDeath1,
		BomberDeath1,
		TurretDeath1
	};
	constexpr int dying_animation_frames_by_type[ENEMY_TYPE_COUNT] = {10, 12, 12, 8};

	dying_sprite.Init(dying_animation_frames_by_type[enemy_type]);
	for (int i = 0; i < dying_animation_frames_by_type[enemy_type]; i++) {
		dying_sprite.images[i] = static_cast<GameImages>(dying_animation_offset_by_type[enemy_type] + i);
	}
	// All death animations take 0.5 seconds.
	dying_sprite.animation_frame_rate = static_cast<float>(dying_animation_frames_by_type[enemy_type]) * 2.0f;
	die_duration = static_cast<float>(dying_animation_frames_by_type[enemy_type]) / dying_sprite.animation_frame_rate;

	state = Walking;
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
	case Dead:
		break;
	}

	for (int i = projectile_count - 1; i >= 0; i--) {
		projectiles[i].Update(game);

		// Remove dead projectiles.
		if (projectiles[i].IsDead()) {
			projectiles[i] = projectiles[projectile_count - 1];
			projectile_count--;
			continue;
		}

		// Check collision against player.
		GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
		Player& player = level->player;

		if (projectiles[i].collider.CheckCollision(player.collider)) {
			player.Hit(damage);
			projectiles[i] = projectiles[projectile_count - 1];
			projectile_count--;
		}
	}

	collider.center = position + Vec2{24, 24};
}

void Enemy::Walk(const Game* game) {
	const GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
	const Player& player = level->player;

	const Vec2 to_player_vector = player.position - position;

	const float distance_from_player_squared = (to_player_vector).MagSquared();

	// Want to check if the sprite needs to flip before we check attack.
	const Vec2 move_direction = NormalizeVec2(to_player_vector);

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

	// If in attack range, we change to attacking state.
	if (distance_from_player_squared < (attack_range * attack_range)) {
		SetState(Attacking);
		return;
	}

	position = position + (move_direction * speed * game->game_time);
}

void Enemy::Attack(Game* game) {
	current_attack_time += game->game_time;

	if (current_attack_time >= attack_duration) {
		GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
		Player& player = level->player;

		// Need to target bottom half of player as that's where the collider is... Maybe should have just don't the whole player
		// But this was the player can walk between 1 tile high gaps.
		const Vec2 attack_direction = NormalizeVec2(player.position + Vec2{0, 16} - position);

		// Probably want to actually do the attack, ie shoot projectile, or check damage collisions.
		switch (type) {
		case Swordsman:
			// If player is in range when attack goes off, damage the player.
			if ((player.position - position).MagSquared() <= attack_range * attack_range) player.Hit(damage);
			SetState(Walking);
			break;
		case Bomber:
			// If in range * 2 damage player
			if ((player.position - position).MagSquared() <= attack_range * attack_range * 4) player.Hit(damage);
			SetState(Dying);
			break;
		case Archer:
		case Turret:
			// Shoot a projectile towards the player.
			if (projectile_count >= max_projectile_count) return;

			projectiles[projectile_count].Init(game);

			projectiles[projectile_count].sprite.Init(4);
			for (int animation_index = 0; animation_index < 4; animation_index++) {
				projectiles[projectile_count].sprite.images[animation_index] = static_cast<GameImages>(ArcherArrow1 + animation_index);
			}
			projectiles[projectile_count].sprite.animation_frame_rate = 16;

			projectiles[projectile_count].Shoot(game, position, attack_direction, 300, 3.0f);
			projectile_count++;
			SetState(Walking);
			break;
		default: ;
		}
		return;
	}
}

void Enemy::Die(const Game* game) {
	current_die_time += game->game_time;
	if (current_die_time >= die_duration) {
		// Need to remove this enemy from the level, Just call a level.QueueRemoveEnemy() function.
		// That will remove enemies end of frame.
		SetState(Dead);
	}
}

void Enemy::Draw(Game* game) const
{
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
	default: ;
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
	default: ;
	}
}

void Enemy::Hit(const float damage_taken) {
	current_health -= damage_taken;

	if (current_health <= 0.0f) {
		SetState(Dying);
	}
}