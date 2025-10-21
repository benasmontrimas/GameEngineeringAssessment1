#include "Sprite.h"

// This doesnt actually need to a pointer to the image, we can just pass the index. Saves a dynamic allocation
// and means we dont need to pass the Game to all the objects with a sprite.
void Sprite::Init(int count) {
	image_count = count;
	images = (GamesEngineeringBase::Image**)malloc(sizeof(GamesEngineeringBase::Image*));
}

GamesEngineeringBase::Image* Sprite::GetImage() const {
	return images[current_frame];
}