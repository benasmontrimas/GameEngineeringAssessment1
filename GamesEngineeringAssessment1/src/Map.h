#pragma once

#include <string>

#include "Vec2.h"
#include "Sprite.h"

class Game;

struct CollisionData {
	Vec2 position{};
};

// Boundary images in this order.
// int Boundaries[16] {
// 	0, // No boundary
// 	1, // bot left
// 	2, // bot right
// 	3, // bot
// 	4, // top right
// 	5, // top right, bottom left
// 	6, // right
// 	7, // all but top left
// 	8, // top left
// 	9, // left
// 	10,// top left, bottom right
// 	11,// all but top right
// 	12,// top
// 	13,// all but bottom right
// 	14,// all but bottom left
// 	15,// all						// INVALID (I don't have a tile for this case)
// }

class TileMap {
	enum TileType : unsigned char {
		Water,

		Grass,
		GrassBL,
		GrassBR,
		GrassB,
		GrassTR,
		GrassTRBL,
		GrassR,
		GrassABTL,
		GrassTL,
		GrassL,
		GrassTLBR,
		GrassABTR,
		GrassT,
		GrassABBR,
		GrassABBL,
		GrassInvalid,

		Path,
		PathBL,
		PathBR,
		PathB,
		PathTR,
		PathTRBL,
		PathR,
		PathABTL,
		PathTL,
		PathL,
		PathTLBR,
		PathABTR,
		PathT,
		PathABBR,
		PathABBL,
		PathInvalid,

		Sand,
		SandBL,
		SandBR,
		SandB,
		SandTR,
		SandTRBL,
		SandR,
		SandABTL,
		SandTL,
		SandL,
		SandTLBR,
		SandABTR,
		SandT,
		SandABBR,
		SandABBL,
		SandInvalid,

		TILE_TYPE_COUNT,
	};

public:
	void Init(Game* game, const std::string& level_path);
	void Update(const Game* game);
	void Draw(Game* game);

	[[nodiscard]] int GetIndex(int x, int y) const;

	static constexpr int map_size = 2000;
	int tile_size = 32;

	unsigned int tiles_in_use = 0;
	unsigned int tiles_in_row = 0;
	int render_x;
	int render_y;

	// Can just create 1 of each sprite that we use and store pointers to those sprites here.
	Sprite tiles[TILE_TYPE_COUNT]; // This is a list of all available tiles.

	int tilemap_width;
	int tilemap_height;
	Sprite** tilemap;
	bool* has_collision;

	// Store all blockable tiles here to reduce checks.
	int collision_data_count;
	CollisionData collision_data[map_size]; // Set to initial size big enough that if the entire screen is has_collision tiles we dont go out of bounds.
};