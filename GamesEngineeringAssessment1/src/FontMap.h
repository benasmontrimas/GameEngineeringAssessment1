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
	void DrawString(Game* game, const std::string& string, const Vec2& position) const;

public:
	GamesEngineeringBase::Image* image{};

	int char_image_width{};
	int char_image_height{};
	int chars_per_line{};
};