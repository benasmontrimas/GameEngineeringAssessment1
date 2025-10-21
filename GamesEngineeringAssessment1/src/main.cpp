#include <iostream>
#include "GamesEngineeringBase.h"
#include "Player.h"
#include "Game.h"

// Make a 1 minute video.

using namespace GamesEngineeringBase;

// TODO: Using too much memory on stack, heap allocate the game or something.
int main() {
	Game game{};
	game.Init();
	game.Run();
}