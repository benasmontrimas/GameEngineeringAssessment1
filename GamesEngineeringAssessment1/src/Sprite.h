#pragma once
#include "GamesEngineeringBase.h"

class Sprite {
public:
	void Init(int count);
	GamesEngineeringBase::Image* GetImage() const;

	int image_count{};
	GamesEngineeringBase::Image** images{ nullptr };
	int depth{};

	int current_frame{};
	float current_frame_duration{0};
	float animation_framerate{};
};