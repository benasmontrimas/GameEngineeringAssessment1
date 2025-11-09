#pragma once

#include "Vec2.h"
#include "Sprite.h"
#include "Collider.h"

class Game;

class PowerUp {
public:
    void Init(Game* game);
    void Update(const Game* game);
    void Draw(Game* game) const;

public:
    Vec2 position;
    Sprite sprite;
    Collider collider;

    static constexpr float life_time{10.0f};
    float time_alive{0.0f};

    bool is_expired{false};
};