#pragma once

#include "CustomBase.h"
#include "Sprite.h"
#include "Collider.h"

class Game;

enum EnemyType {
	Zombie, Archer, Tank, ENEMY_TYPE_COUNT
};

constexpr int ENEMY_HEALTH_BY_TYPE[] = { 100, 50, 200 };

// Just use inheritance.
class Enemy {
public:
	Enemy() = default;

	void Init(Game* game, EnemyType enemy_type);
	void Update(const Game* game);

	EnemyType type;
	Sprite sprite;
	Collider collider{};
	Vec2 position;

	float speed = 50.0f;
	int max_health;
	int current_health;
};