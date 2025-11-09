#pragma once

#include "Vec2.h"
#include "Sprite.h"
#include "Collider.h"

class Game;

class Projectile {
public:
    void Init(Game* game);
    void Shoot(Game* game, const Vec2& start_position, const Vec2& projectile_direction, float projectile_speed, float projectile_life_time);
    void Update(const Game* game);
    void Draw(Game* game) const;

public:
    Collider collider;
    Sprite sprite{};

    Vec2 position;
    Vec2 direction;
    float speed;

    float life_time;
    float current_life_time;

    bool is_dead;
};