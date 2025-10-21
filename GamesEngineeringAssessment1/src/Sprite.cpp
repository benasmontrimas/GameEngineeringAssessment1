#include "Game.h"
#include "Sprite.h"

// This doesnt actually need to a pointer to the image, we can just pass the index. Saves a dynamic allocation
// and means we dont need to pass the Game to all the objects with a sprite.
void Sprite::Init(int count) {
	image_count = count;
	images = (GamesEngineeringBase::Image**)malloc(sizeof(GamesEngineeringBase::Image*));
}

void Sprite::Update(Game* game) {
	current_frame_duration += game->delta_time;
	float time_per_frame = 1.0f / animation_framerate;

	if (current_frame_duration >= time_per_frame) {
		current_frame++;
		current_frame %= image_count;
		current_frame_duration -= time_per_frame;
	}
}

GamesEngineeringBase::Image* Sprite::GetImage() const {
	return images[current_frame];
}

