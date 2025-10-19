#include "Game.h"
void Game::Init() {
	const int WINDOW_WIDTH = 1200;
	const int WINDOW_HEIGHT = 800;
	window.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	timer.reset();

	LoadAssets();

	// Create Depth Array
	depth_buffer = static_cast<int*>(malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT));

	player.Init(this);
	camera.Init();

	camera.SetFollow(&player.position);

	srand(time(0));
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].Init(this, Zombie);
		enemies[i].position.x = rand() % WINDOW_WIDTH;
		enemies[i].position.y = rand() % WINDOW_HEIGHT;
	}

	window_width = window.getWidth();
	window_height = window.getHeight();

	running = true;
}

void Game::Render() {
	// Once we have a background we should be able to ignore clearing, as all the pixels will
	// get over drawn anyway.
	window.clear();
	
	// Clear Depth
	int buffer_size = window_width * window_height;
	//memset(depth_buffer, INT_MIN, buffer_size * sizeof(int));
	for (int i = 0; i < buffer_size; i++) {
		depth_buffer[i] = INT_MAX;
	}

	DrawImage(player.sprite, player.position);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		DrawImage(enemies[i].sprite, enemies[i].position);
	}

	window.present();
}

void Game::Update() {
	player.Update(this);
	camera.Update(delta_time);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].Update(this);
	}
}

void Game::Run() {
	std::cout << running << "\n";

	float count = 0.0f;
	float frame_count = 0;

	while (running) {
		delta_time = timer.dt();

		count += delta_time;
		frame_count++;
		if (count >= 1.0f) {
			std::cout << "FPS: " << frame_count << "\n";
			count = 0.0f;
			frame_count = 0;
		}

		window.checkInput();

		Update();
		Render();

		if (window.keyPressed(VK_ESCAPE)) running = false;

		if (window.keyPressed('W')) player.position.y -= 150 * delta_time;
		if (window.keyPressed('S')) player.position.y += 150 * delta_time;
		if (window.keyPressed('A')) player.position.x -= 150 * delta_time;
		if (window.keyPressed('D')) player.position.x += 150 * delta_time;
	}
}


const char* GAME_IMAGE_PATH[GAME_IMAGE_COUNT] = {
	"resources/Player.png",
	"resources/Zombie.png",
	"resources/Archer.png",
	"resources/Tank.png"
};

void Game::LoadAssets() {
	for (int i = 0; i < GAME_IMAGE_COUNT; i++) {
		images[i].load(GAME_IMAGE_PATH[i]);
	}
}

void Game::DrawImage(const Sprite& sprite, const Vec2& position) {
	// I need image xy, screen xy, world xy
	int world_x_pos_at_0_0 = camera.position.x - (window_width / 2);
	int world_y_pos_at_0_0 = camera.position.y - (window_height / 2);
	
	// Clamp our values to screen limits.
	int image_x_start = max(static_cast<int>(position.x), world_x_pos_at_0_0);
	int image_y_start = max(static_cast<int>(position.y), world_y_pos_at_0_0);

	int image_x_end = min(static_cast<int>(position.x) + static_cast<int>(sprite.image->width), world_x_pos_at_0_0 + window_width);
	int image_y_end = min(static_cast<int>(position.y) + static_cast<int>(sprite.image->height), world_y_pos_at_0_0 + window_height);

	// This is the slow down for rendering enemies, if we sort enemies by their depth, we can
	// remove loads of overdraw.
	for (int image_y = image_y_start; image_y < image_y_end; image_y++) {
		int screen_y = image_y - world_y_pos_at_0_0;
		int image_coord_y = image_y - position.y;
		for (int image_x = image_x_start; image_x < image_x_end; image_x++) {
			int screen_x = image_x - world_x_pos_at_0_0;

			int image_coord_x = image_x - position.x;
			// If transparent skip
			// TODO: Add semi transparent objects.
			// - Would probably want a seperate queue for transparent objects, and render them after all
			//	 opaque sprites. Then we can just do some basic colour calc on whatever colour is on top.

			// std::cout << "Screen X: " << screen_x << ", Screen Y: " << screen_y << "\n";

			// If alpha is 0, skip
			if (sprite.image->alphaAt(image_coord_x, image_coord_y) == 0) continue;

			// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(screen_y * window_width) + screen_x]) continue;

			window.draw(screen_x, screen_y, sprite.image->at(image_coord_x, image_coord_y));
			depth_buffer[(screen_y * 1200) + screen_x] = sprite.depth;
		}
	}
}