#include "Player.h"
#include "Game.h"

void Player::Init(Game* game) {
	sprite.image = &game->images[PlayerBase];
	//std::string filename = "resources/Player.png";
	//sprite.image.load(filename);
}

void Player::Update(Game* game) {
	sprite.depth = -position.y;
}