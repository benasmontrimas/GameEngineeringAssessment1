#include "AOEWeapon.h"
#include "Game.h"
#include "GameImages.h"
#include "Enemy.h"
#include "GameLevel.h"

void AOEWeapon::Init(Game* game) {
    cooldown = 5.0f;
    last_attack = cooldown; // Start off cooldown.
    damage = 200.0f;

    range_ = 200.0f;
    max_enemies_hit_ = 10;

    aoe_sprite_.Init(10);
    for (int i = 0; i < 10; i++) {
        aoe_sprite_.images[i] = static_cast<GameImages>(AOEAttack1 + i);
    }
    aoe_sprite_.animation_frame_rate = 100;
    is_attacking_ = false;
	aoe_sprite_.depth = INT_MAX - 2; // Draw it on the ground.
}

void AOEWeapon::Update(Game* game) {
    last_attack += game->game_time;
    if (last_attack >= cooldown) {
        last_attack = cooldown; // Not sure if needed, but cap in case player doesn't attack for long.
        if (game->window.keyPressed(VK_SPACE)) {
            Attack(game);
            last_attack = 0;
        }
    }

    if (is_attacking_) aoe_sprite_.Update(game);
}

void AOEWeapon::Draw(Game* game) {
    if (!is_attacking_) return;

    attack_animation_timer_ += game->game_time;
    if (attack_animation_timer_ >= static_cast<float>(aoe_sprite_.image_count) / aoe_sprite_.animation_frame_rate) is_attacking_ = false;

    const GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
    const Player& player = level->player;

    // Need to center on player, so move by half the player sprite and then half this sprite.
    game->DrawSprite(aoe_sprite_, player.position + Vec2{.x = 24, .y = 32} - Vec2{.x = 200, .y = 200});
}

void AOEWeapon::Attack(Game* game) {
    is_attacking_ = true;
    attack_animation_timer_ = 0;
    aoe_sprite_.Reset();

    GameLevel* level = dynamic_cast<GameLevel*>(game->GetLevel());
    const Player& player = level->player;
    Enemy* enemies = &level->enemies[0];
    const unsigned int enemies_alive = level->enemies_alive;

    // If we have a power up we can hit twice as many enemies.
    int enemies_to_hit = max_enemies_hit_;
	if (player.power_up_time_left > 0.0f) {
		enemies_to_hit *= 2;
	}

    // Need to store highest max hp enemies, just create array to fit max size we will need.
    Enemy* highest_hp_enemies[20];
    int enemies_hit {0};

    for (unsigned int i = 0; i < enemies_alive; i++) {
        Enemy* enemy = &enemies[i];

        // If enemy is already dying we skip
        if (enemy->state == Enemy::State::Dying) continue;

        // Check in range. Don't need to check if were not in range.
        float distance_from_player_squared = (enemy->position - player.position).MagSquared();
        if (distance_from_player_squared > range_ * range_) continue;

        // Check if higher hp than current selected.
        int hp_index = enemies_hit;
        for (int j = 0; j < enemies_hit; j++) {
            if (enemy->max_health > highest_hp_enemies[j]->max_health) {
                hp_index = j;
                break;
                // If hp is equal select the closest enemy.
            } else if (enemy->max_health == highest_hp_enemies[j]->max_health)
            {
                float current_distance_from_player_squared = (highest_hp_enemies[j]->position - player.position).MagSquared();
	            if (distance_from_player_squared < current_distance_from_player_squared)
	            {
                    hp_index = j;
                    break;
	            }
            }
        }

        // This doesn't make it in the top N max hp so continue;
        if (hp_index >= enemies_to_hit) continue;

        // Move everything below the new position down.
        for (int j = min(enemies_hit, enemies_to_hit - 1); j > hp_index; j--) {
            highest_hp_enemies[j] = highest_hp_enemies[j - 1];
        }

        // Place enemy in array.
        highest_hp_enemies[hp_index] = enemy;
        enemies_hit = min(enemies_hit + 1, enemies_to_hit);
    }

    for (int i = 0; i < enemies_hit; i++) {
        highest_hp_enemies[i]->Hit(damage);
    }
}