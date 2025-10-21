#pragma once
#include "GamesEngineeringBase.h"

class Game;

class Sprite {
public:
	void Init(int count);
	void Update(Game* game);

	GamesEngineeringBase::Image* GetImage() const;

	int image_count{};
	// Does this need to be a pointer to pointer? I guess if we have non
	// consecutive images, then yes, but if all are consecutive, then we can just
	// Point into the original array and specify the stretch.
	GamesEngineeringBase::Image** images{ nullptr };
	int depth{};

	int current_frame{};
	float current_frame_duration{0};
	float animation_framerate{};

	int x_offset[2]{ 0, -1 };
	int y_offset[2]{ 0, -1 };

	bool flip{ false };
};