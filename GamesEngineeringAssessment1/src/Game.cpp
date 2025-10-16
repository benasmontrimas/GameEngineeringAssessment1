#include "Game.h"

void Game::Init() {
	window.create(1200, 800, "Game");
	player.Init();

	running = true;
}

void Game::Render() {
	window.clear();

	int window_width = window.getWidth();
	int window_height = window.getHeight();

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			int sprite_x = static_cast<int>(player.position.x) + x;
			int sprite_y = static_cast<int>(player.position.y) + y;

			if (sprite_x >= window_width or sprite_x < 0 or sprite_y < 0 or sprite_y >= window_height) continue;
			unsigned char* player_colour = player.sprite.at(x, y);
			window.draw(sprite_x, sprite_y, player_colour[0], player_colour[1], player_colour[2]);
		}
	}

	window.present();
}

void Game::Run() {
	std::cout << running << "\n";
	while (running) {
		window.checkInput();

		Render();

		if (window.keyPressed(VK_ESCAPE)) running = false;

		if (window.keyPressed('W')) player.position.y--;
		if (window.keyPressed('S')) player.position.y++;
		if (window.keyPressed('A')) player.position.x--;
		if (window.keyPressed('D')) player.position.x++;
	}
}