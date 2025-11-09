#include "Projectile.h"
#include "Game.h"

void Projectile::Init(Game* game) {
    collider.radius = 1.0f;
}

void Projectile::Shoot(Game* game, const Vec2& start_position, const Vec2& projectile_direction, const float projectile_speed, const float projectile_life_time) {
    sprite.Reset();

    position = start_position;
    direction = projectile_direction;
    speed = projectile_speed;
    life_time = projectile_life_time;

    is_dead = false;
    current_life_time = 0.0f;
}

void Projectile::Update(const Game* game) {
    sprite.Update(game);

    position = position + (direction * speed * game->game_time);
    current_life_time += game->game_time;

    if (current_life_time >= life_time) {
        is_dead = true;
    }

    collider.center = position + Vec2{static_cast<float>(game->images[sprite.GetImage()].width) / 2.0f, static_cast<float>(game->images[sprite.GetImage()].height) / 2.0f};
}

void Projectile::Draw(Game* game) const
{
    const float angle = atan2(direction.x, -direction.y);

    game->DrawSprite(sprite, position, angle, {24, 32});
}
