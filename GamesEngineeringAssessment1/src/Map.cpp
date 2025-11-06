#include "Map.h"
#include "Game.h"
#include <cassert>
#include <fstream>

void TileMap::Init(Game* game, std::string level_path) {
	GameImages image_offset[TILE_TYPE_COUNT] = {
		Water1,
		Grass1,
		Path1,
	};

	int frame_count[TILE_TYPE_COUNT] = {
		8,
		1,
		1,
	};

	for (int type = Water; type < TILE_TYPE_COUNT; type++) {
		tiles[type].Init(frame_count[type]);
		for (int i = 0; i < frame_count[type]; i++) {
			tiles[type].images[i] = &game->images[image_offset[type] + i];
		}
		tiles[type].animation_framerate = 8;
		tiles[type].depth = INT_MAX - 1;
	}

	// Read file
	// Set tilemap to read files, then when we draw we just need to get the modulus of the index and use the tile
	// From tilemap. well for infinite atleast, for fixed size, we just need to set camera boundry.

	// If no file is passed we just create a water only map. (Used for main menu).
	if (level_path == "") {
		tilemap_width = 1;
		tilemap_height = 1;
		tilemap = new Sprite*[1];
		collidable = new bool[1];
		tilemap[0] = &tiles[0];
		return;
	}

	std::fstream file{level_path, file.in};
	if (!file.is_open()) {
		std::cout << "ERROR: failed reading level file - " << level_path << "\n";
		return;
	}

	std::string line;

	std::getline(file, line);

	if (line[0] == 'S') {
		// Static Map
	} else if (line[0] == 'I') {
		// Infinite Map
	}

	std::getline(file, line);
	tilemap_width = std::stoi(line);

	std::getline(file, line);
	tilemap_height = std::stoi(line);

	tilemap = new Sprite*[tilemap_width * tilemap_height];
	collidable = new bool[tilemap_width * tilemap_height];

	for (int y = 0; y < tilemap_height; y++) {
		std::getline(file, line);
		for (int x = 0; x < tilemap_width; x++) {
			collidable[x + (y * tilemap_width)] = false; // Default to false
			switch (line[x]) {
			case 'W':
				tilemap[x + (y * tilemap_width)] = &tiles[Water];
				collidable[x + (y * tilemap_width)] = true;
				break;
			case 'P':
				tilemap[x + (y * tilemap_width)] = &tiles[Path];
				break;
			case 'G':
				tilemap[x + (y * tilemap_width)] = &tiles[Grass];
				break;
			}
		}
	}
}

void TileMap::Update(Game* game) {
	// Need to get all tiles visible to camera and load them, and unload all others.
	// Only ever need (window_width / tile_size) + 1 tiles. (well I thought, still have gaps, so did +2).

	for (int i = 0; i < TILE_TYPE_COUNT; i++) {
		tiles[i].Update(game);
	}
}

void TileMap::Draw(Game* game)
{
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

	while (start_index_x < 0) start_index_x += tilemap_width;
	int tilemap_index_x = start_index_x % tilemap_width;

	while (start_index_y < 0) start_index_y += tilemap_height;
	int tilemap_index_y = start_index_y % tilemap_height;

	// Reset collision data, so we can recalc every frame
	collision_data_count = 0;
	for (unsigned int y = 0; y < tiles_needed_y; y++) {
		for (unsigned int x = 0; x < tiles_needed_x; x++) {
			const float x_pos = static_cast<float>(render_x) + (static_cast<float>(x) * static_cast<float>(tile_size));
			const float y_pos = static_cast<float>(render_y) + (static_cast<float>(y) * static_cast<float>(tile_size));

			int final_x = (start_index_x + x) % tilemap_width;
			int final_y = (start_index_y + y) % tilemap_height;
			Sprite* sprite_to_draw = tilemap[final_x + (final_y * tilemap_width)];

			game->DrawSprite(*sprite_to_draw, Vec2{x_pos, y_pos});

			// check if not collidable and continue
			if (!collidable[final_x + (final_y * tilemap_width)]) continue;
			// else add to collision data
			collision_data[collision_data_count].position = Vec2{x_pos, y_pos};
			collision_data_count++;

		}
	}
}
