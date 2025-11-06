#pragma once

#include "CustomBase.h"
#include "Sprite.h"
#include "Collider.h"
#include "Projectile.h"

class Game;
class Player;

// OK i want a normal zombie, archer, turret, and bomber, no tank. Bomber will run at player and on collision blow up and deal damage.
enum EnemyType {
	Swordsman, Archer, Bomber, Turret, ENEMY_TYPE_COUNT
};

static constexpr int ENEMY_HEALTH_BY_TYPE[] = { 100, 50, 50, 100 };
static constexpr int ENEMY_ATTACK_RANGE_BY_TYPE[] = { 40, 200, 0, 400 };

class Enemy {
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
	void Deinit();
	void Update(Game* game);
	void Draw(Game* game);

	void Walk(Game* game);
	void Attack(Game* game);
	void Die(Game* game);

	void Hit(float damage);

	void SetState(const State new_state);

	Player* player;

	EnemyType type;
	Sprite sprite{};
	Sprite attacking_sprite{};
	Sprite dying_sprite{};
	Collider collider{};
	Vec2 position;

	State state;
	float attack_range;
	float speed = 50.0f;
	int max_health;
	int current_health;

	float attack_duration;
	float current_attack_time;

	float die_duration;
	float current_die_time;

	int projectile_count = 0;
	static constexpr int max_projectile_count = 10;
	Projectile projectiles[max_projectile_count];
};