#include "Map.h"
#include "Game.h"
#include <cassert>

void TileMap::Init(Game* game) {
	for (int i = 0; i < MAP_SIZE; i++) {
		tilemap[i].Init(1);

		const int TILE_COUNT = 11;
		int rand_offset = rand() % TILE_COUNT;
		tilemap[i].images[0] = &game->images[Tile0 + rand_offset];

		tilemap[i].depth = INT_MAX - 1;
	}
}

void TileMap::Update(Game* game) {
	// Need to get all tiles visible to camera and load them, and unload all others.
	// Only ever need (window_width / tile_size) + 1 tiles. (well i thought, still have gaps, so did +2).

	Vec2 camera_position = game->camera.position;
	int screen_x = static_cast<int>(camera_position.x) - (game->window_width / 2);
	int screen_y = static_cast<int>(camera_position.y) - (game->window_height / 2);

	int start_index_x = screen_x / 32;
	if (screen_x < 0) start_index_x--; // As float round to 0, need to -1 to prevent gaps when in negative coords.

	int start_index_y = screen_y / 32;
	if (screen_y < 0) start_index_y--;

	int tiles_needed_x = (game->window_width / 32) + 2;
	int tiles_needed_y = (game->window_height / 32) + 2;

	tiles_in_use = tiles_needed_x * tiles_needed_y;
	assert(tiles_in_use <= MAP_SIZE); // Assert that we have allocated enough space for our tiles. tbf we could allocate this exact.

	tiles_in_row = tiles_needed_x;
	render_x = start_index_x * 32;
	render_y = start_index_y * 32;

	int index = 0;
	for (int y = start_index_y; y < start_index_y + tiles_needed_y; y++) {
		for (int x = start_index_x; x < start_index_x + tiles_needed_x; x++) {
			// Need to get which tile we want to load.
			tilemap[index].images[0] = &game->images[Tile0 + ((unsigned int(y) * 41534421 + (unsigned int(x) + unsigned int(y) * 2001032 ) * 123242113) % 11)];
			index++;
		}
	}
}