#pragma once
#include "GameImages.h"

class Game;

class Sprite {
public:
	void Init(int count);
	void Update(const Game* game);

	void Reset();

	[[nodiscard]] GameImages GetImage() const;

	int image_count{};
	// Does this need to be a pointer to pointer? I guess if we have non-consecutive
	// images, then yes, but if all are consecutive, then we can just
	// Point into the original array and specify the stretch.
	// NOTE: Needs to be manually saved/loaded.
	GameImages images[18];
	int depth{};

	int current_frame{};
	float current_frame_duration{0};
	float animation_frame_rate{};

	unsigned char modulation_colour[3]{ 255, 255, 255 }; // Use to alter final colour.

	int x_offset[2]{ 0, -1 };
	int y_offset[2]{ 0, -1 };

	bool flip{ false };
};