#pragma once

#include "Weapon.h"
#include "Sprite.h"

class Game;

class AOEWeapon : public Weapon {
public:
    void Init(Game* game) override;
    void Attack(Game* game) override;
    void Update(Game* game) override;
    void Draw(Game* game) override;

private:
    float range_{};
    int max_enemies_hit_{};

    Sprite aoe_sprite_{};

    float attack_animation_timer_{};
    bool is_attacking_{};
};