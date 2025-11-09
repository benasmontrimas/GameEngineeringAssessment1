#include "PowerUp.h"
#include "Game.h"

void PowerUp::Init(Game* game) {
    sprite.Init(8);
    for (int i = 0; i < 8; i++) {
        sprite.images[i] = static_cast<GameImages>(PowerUp1 + i);
    }
    sprite.animation_frame_rate = 16;

    collider.radius = 16;
}

void PowerUp::Update(const Game* game) {
    collider.center = position + Vec2{16, 16};
    sprite.Update(game);

    time_alive += game->game_time;
    if (time_alive >= life_time) {
        is_expired = true;
    }
}

void PowerUp::Draw(Game* game) const
{
    game->DrawSprite(sprite, position);
}
