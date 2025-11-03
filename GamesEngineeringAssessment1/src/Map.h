#pragma once

#include "Sprite.h"

class Game;

// Need to be able to load tile sets from files. just create arrays of ints to specify offsets into the sprite array
// signifying which tile to use. Can add header to specify which tile set to use, allows changing tiles for same set.
class TileMap {
public:
	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game);

		static constexpr int map_size = 10000;
	static constexpr int map_width = 100;
	int tile_size = 32;

	unsigned int tiles_in_use = 0;
	unsigned int tiles_in_row = 0;
	int render_x;
	int render_y;

	Sprite tilemap[map_size];
};