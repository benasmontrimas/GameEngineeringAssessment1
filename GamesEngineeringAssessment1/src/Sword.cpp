#include "Sword.h"
#include "Game.h"
#include "Enemy.h"

Sword::Sword()
{
	type = WeaponType::AutoAttack;

	name = "Sword";
	description = "A fast swinging auto attack.";

	cooldown = 1.0f;
	damage = 10.0f;

	attack_duration = 0.25f;

	// Need to set sprites, don't have any yet;
	//attack_animation
	//sprite
	//icon

	is_attacking = false;
}

void Sword::Attack() {
	// Set collider to sword position
	// Check if it collides, need to keep list of hit enemies so were not hitting multiple times with the same attack.
	
	is_attacking = true;
}

void Sword::Update(Game* game) {
	if (!is_attacking) {
		last_attack += game->game_time;
	}
	else {
		// How do we make a round collider, without overcomplicating the collider math...
		// we can do some round colliders and take slices from them. might be best bet.
		// Update sword swing arc
		// check collisions from collider
		// add all enemies to list of hit enemies, needs to be cleared on finish attack.

		current_attack_time += game->game_time;
		if (current_attack_time > attack_duration) {
			is_attacking = false;
			enemies_hit_count = 0; // Don't actually need to clear the array.
		}
	}
}