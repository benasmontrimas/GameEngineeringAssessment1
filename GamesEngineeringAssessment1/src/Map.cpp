#include "Map.h"
#include "Game.h"
#include <cassert>

void TileMap::Init(Game* game) {
	for (int i = 0; i < map_size; i++) {
		tilemap[i].Init(1);
		tilemap[i].depth = INT_MAX - 1;
	}
}

void TileMap::Update(Game* game) {
	// Need to get all tiles visible to camera and load them, and unload all others.
	// Only ever need (window_width / tile_size) + 1 tiles. (well I thought, still have gaps, so did +2).

	const Vec2 camera_position = game->camera.position;
	const int screen_x = static_cast<int>(camera_position.x) - static_cast<int>(game->window_width / 2);
	const int screen_y = static_cast<int>(camera_position.y) - static_cast<int>(game->window_height / 2);

	int start_index_x = screen_x / 32;
	if (screen_x < 0) start_index_x--; // As float round to 0, need to -1 to prevent gaps when in negative coords.

	int start_index_y = screen_y / 32;
	if (screen_y < 0) start_index_y--;

	const unsigned int tiles_needed_x = (game->window_width / 32) + 2;
	const unsigned int tiles_needed_y = (game->window_height / 32) + 2;

	tiles_in_use = tiles_needed_x * tiles_needed_y;
	assert(tiles_in_use <= map_size); // Assert that we have allocated enough space for our tiles. tbf we could allocate this exact.

	tiles_in_row = tiles_needed_x;
	render_x = start_index_x * 32;
	render_y = start_index_y * 32;

	int index = 0;
	for (int y = start_index_y; y < start_index_y + static_cast<int>(tiles_needed_y); y++) {
		for (int x = start_index_x; x < start_index_x + static_cast<int>(tiles_needed_x); x++) {
			tilemap[index].images[0] = &game->images[Tile0];
			index++;
		}
	}
}

void TileMap::Draw(Game* game)
{
	for (unsigned int i = 0; i < tiles_in_use; i++) {
		const float x = static_cast<float>(render_x) + (static_cast<float>(i % tiles_in_row) * static_cast<float>(tile_size));
		const float y = static_cast<float>(render_y) + (static_cast<float>(i / tiles_in_row) * static_cast<float>(tile_size));
		game->DrawSprite(tilemap[i], Vec2{ .x = x, .y = y });
	}
}
