#pragma once

#include "CustomBase.h"
#include "Sprite.h"
#include "Collider.h"

class Game;
class Player;

// OK i want a normal zombie, archer, turret, and bomber, no tank. Bomber will run at player and on collision blow up and deal damage.
enum EnemyType {
	Zombie, Archer, Bomber, Turret, ENEMY_TYPE_COUNT
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
	void Update(const Game* game);
	void Draw(Game* game);

	void Walk(const Game* game);
	void Attack(const Game* game);
	void Die(const Game* game);

	void Hit(float damage);

	void SetState(const State new_state);

	Player* player;

	EnemyType type;
	Sprite sprite;
	Sprite attacking_sprite;
	Sprite dying_sprite;
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
};