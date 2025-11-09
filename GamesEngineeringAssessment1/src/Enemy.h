#pragma once

#include "Vec2.h"
#include "Sprite.h"
#include "Collider.h"
#include "Projectile.h"

class Game;
class Player;

enum EnemyType : unsigned char {
	Swordsman, Archer, Bomber, Turret, ENEMY_TYPE_COUNT
};

class Enemy {
	static constexpr float enemy_health_by_type[ENEMY_TYPE_COUNT] = { 150, 50, 75, 100 };
	static constexpr float enemy_attack_range_by_type[ENEMY_TYPE_COUNT] = { 40, 200, 20, 400 };
	static constexpr float enemy_speed_by_type[ENEMY_TYPE_COUNT] = { 50, 30, 70, 0 };
	static constexpr float enemy_damage_by_type[ENEMY_TYPE_COUNT] = { 20, 10, 50, 10 };
public:
	enum State : unsigned char {
		Walking, // We use the walking state as an idle state for the turret.
		Attacking,
		Dying,
		Dead,
	};

public:
	Enemy() = default;

	void Init(Game* game, EnemyType enemy_type);
	void Update(Game* game);
	void Draw(Game* game) const;

	void Walk(const Game* game);
	void Attack(Game* game);
	void Die(const Game* game);

	void Hit(float damage_taken);

	void SetState(const State new_state);

	EnemyType type;
	Sprite sprite{};
	Sprite attacking_sprite{};
	Sprite dying_sprite{};
	Collider collider{};
	Vec2 position;

	State state;
	float attack_range;
	float speed;
	float damage;
	float max_health;
	float current_health;

	float attack_duration;
	float current_attack_time;

	float die_duration;
	float current_die_time;

	int projectile_count = 0;
	static constexpr int max_projectile_count = 10;
	Projectile projectiles[max_projectile_count];
};