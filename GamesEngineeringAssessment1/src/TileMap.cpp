#include "TileMap.h"
#include "Game.h"
#include <cassert>
#include <fstream>

TileMap::~TileMap()
{
	delete[] has_collision;
	delete[] tilemap;
}

void TileMap::Init(Game* game, const std::string& level_path) {
	GameImages image_offset[TILE_TYPE_COUNT] = {
		Water1,

		Grass1,
		GrassBL1,
		GrassBR1,
		GrassB1,
		GrassTR1,
		GrassTRBL1,
		GrassR1,
		GrassABTL1,
		GrassTL1,
		GrassL1,
		GrassTLBR1,
		GrassABTR1,
		GrassT1,
		GrassABBR1,
		GrassABBL1,
		TileInvalid1,

		Path1,
		PathBL1,
		PathBR1,
		PathB1,
		PathTR1,
		PathTRBL1,
		PathR1,
		PathABTL1,
		PathTL1,
		PathL1,
		PathTLBR1,
		PathABTR1,
		PathT1,
		PathABBR1,
		PathABBL1,
		TileInvalid1,

		Sand1,
		SandBL1,
		SandBR1,
		SandB1,
		SandTR1,
		SandTRBL1,
		SandR1,
		SandABTL1,
		SandTL1,
		SandL1,
		SandTLBR1,
		SandABTR1,
		SandT1,
		SandABBR1,
		SandABBL1,
		TileInvalid1,
	};

	for (int type = Water; type < TILE_TYPE_COUNT; type++) {
		int frame_count[TILE_TYPE_COUNT] = {
			8,

			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,

			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,

			1,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			15,
			1,
		};

		tiles[type].Init(frame_count[type]);
		for (int i = 0; i < frame_count[type]; i++) {
			tiles[type].images[i] = static_cast<GameImages>(image_offset[type] + i);
		}
		tiles[type].animation_frame_rate = 8;
		tiles[type].depth = INT_MAX - 1;
	}

	// Read file
	// Set tilemap to read files, then when we draw we just need to get the modulus of the index and use the tile
	// From tilemap. well for infinite atleast, for fixed size, we just need to set camera boundry.

	// If no file is passed we just create a water only map. (Used for main menu (Well wan before I added some islands)).
	if (level_path.empty()) {
		tilemap_width = 1;
		tilemap_height = 1;
		tilemap = new Sprite*[1];
		has_collision = new bool[1];
		tilemap[0] = &tiles[0];
		return;
	}

	std::fstream file{level_path, std::fstream::in};
	if (!file.is_open()) {
		std::cout << "ERROR: failed reading level file - " << level_path << "\n";
		return;
	}

	std::string line;

	std::getline(file, line);

	if (line[0] == 'S') {
		// Static Map
		game->camera.capped_to_boundary = true;
	} else if (line[0] == 'I') {
		// Infinite Map
		game->camera.capped_to_boundary = false;
	}

	std::getline(file, line);
	tilemap_width = std::stoi(line);

	std::getline(file, line);
	tilemap_height = std::stoi(line);

	tilemap = new Sprite*[static_cast<size_t>(tilemap_width * tilemap_height)];
	has_collision = new bool[static_cast<size_t>(tilemap_width * tilemap_height)];

	TileType* map_layout = new TileType[static_cast<size_t>(tilemap_width * tilemap_height)];

	for (int y = 0; y < tilemap_height; y++) {
		std::getline(file, line);
		for (int x = 0; x < tilemap_width; x++) {
			has_collision[x + (y * tilemap_width)] = false; // Default to false
			switch (line[x]) {
			case 'W':
				map_layout[x + (y * tilemap_width)] = Water;
				has_collision[x + (y * tilemap_width)] = true;
				break;
			case 'P':
				map_layout[x + (y * tilemap_width)] = Path;
				break;
			case 'G':
				map_layout[x + (y * tilemap_width)] = Grass;
				break;
			case 'S':
				map_layout[x + (y * tilemap_width)] = Sand;
				break;
			default: ;
				std::cout << "Unknown tile type in file! Defaulting to Water\n";
				map_layout[x + (y * tilemap_width)] = Water;
				has_collision[x + (y * tilemap_width)] = true;
				break;
			}
		}
	}

	// Need to look for adjacency on tiles so we can choose the correct tile to draw.
	for (int y = 0; y < tilemap_height; y++) {
		for (int x = 0; x < tilemap_width; x++) {
			constexpr int bl = 1;
			constexpr int br = 2;
			constexpr int tr = 4;
			constexpr int tl = 8;

			int lookup_index = 0;

			int index = GetIndex(x, y);

			// Water is the base, has no boundaries
			if (map_layout[index] == Water) {
				tilemap[index] = &tiles[map_layout[index]];
				continue;
			}

			bool tl_in = false;
			bool tr_in = false;
			bool br_in = false;
			bool bl_in = false;

			TileType tile_to_look_for;

			// Tile only look for 1 type of tile to be adjacent to.
			// Sand reacts to water, doesn't need to react to grass as grass reacts to the sand... so on.
			switch (map_layout[index]) {
			case Grass:
				tile_to_look_for = Sand;
				break;
			case Path:
				tile_to_look_for = Grass;
				break;
			case Sand:
				tile_to_look_for = Water;
				break;
			default:
				std::cout << "Unexpected tile!\n";
				tile_to_look_for = Water; // Should never hit but initialise this just in case.
				break;
			}

			// If were at the boundary loop around
			int x_minus_1 = ((x - 1) + tilemap_width) % tilemap_width;
			int y_minus_1 = ((y - 1) + tilemap_height) % tilemap_height;
			int x_plus_1 = (x + 1) % tilemap_width;
			int y_plus_1 = (y + 1) % tilemap_height;

			int tl_index = GetIndex(x_minus_1, y_minus_1);
			int t_index = GetIndex(x, y_minus_1);
			int tr_index = GetIndex(x_plus_1, y_minus_1);

			int l_index = GetIndex(x_minus_1, y);
			int r_index = GetIndex(x_plus_1, y);

			int bl_index = GetIndex(x_minus_1, y_plus_1);
			int b_index = GetIndex(x, y_plus_1);
			int br_index = GetIndex(x_plus_1, y_plus_1);

			// check instances and mark corners as in
			if (map_layout[tl_index] == tile_to_look_for) tl_in = true;
			if (map_layout[t_index] == tile_to_look_for) {
				tl_in = true;
				tr_in = true;
			}
			if (map_layout[tr_index] == tile_to_look_for) tr_in = true;

			if (map_layout[l_index] == tile_to_look_for) {
				tl_in = true;
				bl_in = true;
			}
			if (map_layout[r_index] == tile_to_look_for) {
				tr_in = true;
				br_in = true;
			}

			if (map_layout[bl_index] == tile_to_look_for) bl_in = true;
			if (map_layout[b_index] == tile_to_look_for) {
				bl_in = true;
				br_in = true;
			}
			if (map_layout[br_index] == tile_to_look_for) br_in = true;

			if (tl_in) lookup_index += tl;
			if (tr_in) lookup_index += tr;
			if (br_in) lookup_index += br;
			if (bl_in) lookup_index += bl;

			tilemap[index] = &tiles[map_layout[index] + lookup_index];
		}
	}

	delete[] map_layout;

	float camera_offset_x = static_cast<float>(game->window_width) / 2.0f;
	float camera_offset_y = static_cast<float>(game->window_height) / 2.0f;
	game->camera.min_position = Vec2{camera_offset_x, camera_offset_y};
	game->camera.max_position = Vec2{static_cast<float>(tilemap_width * tile_size) - camera_offset_x,  static_cast<float>(tilemap_height * tile_size) - camera_offset_y};
}

void TileMap::Update(const Game* game) {
	for (auto& tile : tiles)
	{
		tile.Update(game);
	}
}

void TileMap::Draw(Game* game)
{
	// This is where we actually calculate the positions of tiles.
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

	// Just to make sure were in the positives so when we modulo we get an index.
	while (start_index_x < 0) start_index_x += tilemap_width;
	while (start_index_y < 0) start_index_y += tilemap_height;

	// Reset collision data, so we can recalc every frame
	collision_data_count = 0;
	for (unsigned int y = 0; y < tiles_needed_y; y++) {
		for (unsigned int x = 0; x < tiles_needed_x; x++) {
			const float x_pos = static_cast<float>(render_x) + (static_cast<float>(x) * static_cast<float>(tile_size));
			const float y_pos = static_cast<float>(render_y) + (static_cast<float>(y) * static_cast<float>(tile_size));

			const int final_x = (start_index_x + static_cast<int>(x)) % tilemap_width;
			const int final_y = (start_index_y + static_cast<int>(y)) % tilemap_height;
			const Sprite* sprite_to_draw = tilemap[GetIndex(final_x, final_y)];

			game->DrawSprite(*sprite_to_draw, Vec2{x_pos, y_pos});

			// check if a tile with collision if not continue
			if (!has_collision[GetIndex(final_x, final_y)]) continue;
			// else add to collision data
			collision_data[collision_data_count].position = Vec2{x_pos, y_pos};
			collision_data_count++;

		}
	}
}

int TileMap::GetIndex(const int x, const int y) const
{
	return x + (y * tilemap_width);
}
