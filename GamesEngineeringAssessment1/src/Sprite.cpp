#include "Game.h"
#include "Sprite.h"

// This doesn't actually need to a pointer to the image, we can just pass the index. Saves a dynamic allocation
// and means we don't need to pass the Game to all the objects with a sprite.
void Sprite::Init(const int count) {
	image_count = count;
	images = static_cast<GamesEngineeringBase::Image**>(malloc(sizeof(GamesEngineeringBase::Image*) * image_count));
}

void Sprite::Update(const Game* game) {
	current_frame_duration += game->game_time;
	float time_per_frame = 1.0f / animation_framerate;

	if (current_frame_duration >= time_per_frame) {
		current_frame++;
		current_frame %= image_count;
		current_frame_duration -= time_per_frame;
	}
}

void Sprite::Reset() {
	current_frame = 0;
	current_frame_duration = 0;
}

GamesEngineeringBase::Image* Sprite::GetImage() const {
	return images[current_frame];
}

