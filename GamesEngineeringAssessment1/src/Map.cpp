#include "Map.h"
#include "Game.h"

void TileMap::Init(Game* game) {
	for (int i = 0; i < MAP_SIZE; i++) {
		tilemap[i].Init(1);

		const int TILE_COUNT = 11;
		int rand_offset = rand() % TILE_COUNT;
		tilemap[i].images[0] = &game->images[Tile0 + rand_offset];

		tilemap[i].depth = INT_MAX - 1;
	}
}