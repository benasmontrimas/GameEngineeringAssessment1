#include "GamesEngineeringBase.h"
#include "FontMap.h"
#include "Sprite.h"
#include "Game.h"

void FontMap::Init(GamesEngineeringBase::Image* _image, int w, int h, int _chars_per_line) {
	image = _image;
	char_image_width = w;
	char_image_height = h;
	chars_per_line = _chars_per_line;
}

void FontMap::DrawString(Game* game, std::string string, const Vec2& position) {
	Sprite char_sprite;
	char_sprite.Init(1);
	char_sprite.images[0] = image;
	char_sprite.depth = INT_MIN;

	for (int i = 0; i < string.size(); i++) {
		char c = string[i];
		if (c == ' ') continue;

		int index_into_font_map = c - '!';

		int x = char_image_width * (index_into_font_map % chars_per_line);
		int y = char_image_height * (index_into_font_map / chars_per_line);

		char_sprite.x_offset[0] = x + 1;
		char_sprite.x_offset[1] = x + char_image_width;

		char_sprite.y_offset[0] = y + 1;
		char_sprite.y_offset[1] = y + char_image_height;

		game->DrawImage(char_sprite, position + Vec2{ float(i * char_image_width) * 0.6f, 0.0f });
	};

}