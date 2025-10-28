#include "GamesEngineeringBase.h"
#include "FontMap.h"
#include "Sprite.h"
#include "Game.h"

void FontMap::Init(GamesEngineeringBase::Image* _image, const int w, const int h, const int _chars_per_line) {
	image = _image;
	char_image_width = w;
	char_image_height = h;
	chars_per_line = _chars_per_line;
}

// TODO: Can add support for new line char, just need to reset x offset and set y offset to the height of the chars.
void FontMap::DrawString(Game* game, const std::string& string, const Vec2& position) const
{
	Sprite char_sprite;
	char_sprite.Init(1);
	char_sprite.images[0] = image;
	char_sprite.depth = INT_MIN;

	for (int i = 0; i < string.size(); i++) {
		char c = string[i];
		if (c == ' ') continue;
		if (c >= 'a' and c <= 'z') c -= 'a' - 'A'; // Set to the upper case equivalent letter as we don't support lower case.
		if (c < '!' or c > 'Z') c = 'Z' + 1; // IF it's an unsupported char, set to one of the invalid char lists;

		const int index_into_font_map = c - '!';

		int x = char_image_width * (index_into_font_map % chars_per_line);
		int y = char_image_height * (index_into_font_map / chars_per_line);

		char_sprite.x_offset[0] = x + 1;
		char_sprite.x_offset[1] = x + char_image_width;

		char_sprite.y_offset[0] = y + 1;
		char_sprite.y_offset[1] = y + char_image_height;

		game->DrawSprite(char_sprite, position + Vec2{ static_cast<float>(i * char_image_width) * 0.6f, 0.0f });
	};

}
