#pragma once

#include "Sprite.h"

class Game;

// Need to be able to load tile sets from files. just create arrays of ints to specify offsets into the sprite array
// signifying which tile to use. Can add header to specify which tile set to use, allows changing tiles for same set.
class TileMap {
public:
	void Init(Game* game);

	static const int MAP_SIZE = 10000;
	const int MAP_WIDTH = 100;
	int tile_size = 32;

	Sprite tilemap[MAP_SIZE];
};