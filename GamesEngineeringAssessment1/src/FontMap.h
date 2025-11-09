#pragma once

#include <string>
#include "Vec2.h"
#include "Sprite.h"
#include "GameImages.h"

class Game;

namespace GamesEngineeringBase {
	class Image;
}

class FontMap {
public:
	void Init(GameImages img, int w, int h, int chars_per_row);
	void DrawString(Game* game, const std::string& string, const Vec2& position);

public:
	GameImages image{};

	Sprite char_sprite{};

	int char_image_width{};
	int char_image_height{};
	int chars_per_line{};
};