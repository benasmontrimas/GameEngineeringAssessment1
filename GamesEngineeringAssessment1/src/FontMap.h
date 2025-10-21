#pragma once

#include <string>
#include "CustomBase.h"

class Game;

namespace GamesEngineeringBase {
	class Image;
}

class FontMap {
public:
	void Init(GamesEngineeringBase::Image* image, int w, int h, int _chars_per_line);
	void DrawString(Game* game, std::string string, const Vec2& position);

public:
	GamesEngineeringBase::Image* image{};

	int char_image_width{};
	int char_image_height{};
	int chars_per_line{};
};