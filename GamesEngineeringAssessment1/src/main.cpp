#include <iostream>
#include "GamesEngineeringBase.h"
#include "Player.h"
#include "Game.h"

// Make a 1 minute video.

using namespace GamesEngineeringBase;

int main() {

	Game game{};
	game.Init();
	game.Run();
}