#include "Game.h"
#include <cassert>

#include "MainMenuLevel.h"

// Want to add some UI, health, what weapons are equiped. Enemies killed. Time.

void Game::Init() {
	// Set a random seed at the start of the game.
	srand(static_cast<int>(time(nullptr)));

	constexpr int WINDOW_WIDTH = 1200;
	constexpr int WINDOW_HEIGHT = 800;
	window.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	window_width = window.getWidth();
	window_height = window.getHeight();

	// Needs to be after we create window.
	LoadAssets();

	depth_buffer = static_cast<int*>(malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT));

	level_map.Init(this);
	player.Init(this);
	camera.Init();
	hud.Init(this);

	font32.Init(&images[Font32Pt], 32, 32, 8);
	font16.Init(&images[Font16Pt], 16, 16, 8);

	camera.SetFollow(&player.position);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].Init(this, Zombie);
	}

	running = true;
	timer.reset();

	// TESTING;
	active_level = new MainMenuLevel;
	active_level->Init(this);
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

		elapsed_seconds += delta_time;

		window.checkInput();

		Update();
		Render();

		if (window.keyPressed(VK_ESCAPE)) running = false;
	}
}

void Game::SpawnEnemy() {
	Vec2 spawn_vector{};
	const int rand_x = rand() % 2000;
	const int rand_y = rand() % 2000;
	spawn_vector.x = static_cast<float>(rand_x - 1000) / 1000.0f;
	spawn_vector.y = static_cast<float>(rand_y - 1000) / 1000.0f;

	const Vec2 normalized_spawn_vector = NormalizeVec2(spawn_vector);
	float multiplier = 0.0f;
	if (abs(window_width / normalized_spawn_vector.x) < abs(window_height / normalized_spawn_vector.y)) {
		// sides closer
		multiplier = static_cast<float>(window_width) / normalized_spawn_vector.x;
	}
	else {
		// top/bottom closer
		multiplier = static_cast<float>(window_height) / normalized_spawn_vector.y;
	}

	enemies[enemies_alive].position = camera.position + (normalized_spawn_vector * abs(multiplier));
	enemies_alive++;
}

void Game::Update() {

	if (last_spawn != static_cast<int>(elapsed_seconds / spawn_cooldown)) {
		int number_of_enemies_to_spawn = MAX_ENEMIES / static_cast<int>(level_duration / (elapsed_seconds));
		number_of_enemies_to_spawn = min(number_of_enemies_to_spawn + 10, MAX_ENEMIES - enemies_alive);

		for (int i = 0; i < number_of_enemies_to_spawn; i++) {
			SpawnEnemy();
		}

		std::cout << number_of_enemies_to_spawn << "\n";

		last_spawn = static_cast<int>(elapsed_seconds / spawn_cooldown);
	}

	player.Update(this);
	camera.Update(delta_time);
	level_map.Update(this);

	int enemies_to_remove = 0;
	int enemy_to_remove[MAX_ENEMIES];

	for (int i = 0; i < enemies_alive; i++) {
		// Collision
		const bool collided_with_player = enemies[i].collider.CheckCollision(player.collider);
		if (collided_with_player) {
			enemy_to_remove[enemies_to_remove] = i;
			enemies_to_remove++;
		}
		enemies[i].Update(this);
	}

	for (int i = enemies_to_remove - 1; i >= 0; i--) {
		enemies[enemy_to_remove[i]] = enemies[enemies_alive - 1];
		enemies_alive--;
	}

	if (active_level) active_level->Update(this);
}

void Game::Render() {
	// Once we have a background we should be able to ignore clearing, as all the pixels will
	// get over drawn anyway.
	// window.clear();

	// Clear Depth
	const unsigned int buffer_size = window_width * window_height;
	// memset(depth_buffer, INT_MIN, buffer_size * sizeof(int));
	for (unsigned int i = 0; i < buffer_size; i++) {
		depth_buffer[i] = INT_MAX;
	}

	for (int i = 0; i < level_map.tiles_in_use; i++) {
		const float x = static_cast<float>(level_map.render_x) + (static_cast<float>(i % level_map.tiles_in_row) * static_cast<float>(level_map.tile_size));
		const float y = static_cast<float>(level_map.render_y) + (static_cast<float>(i / level_map.tiles_in_row) * static_cast<float>(level_map.tile_size));
		DrawSprite(level_map.tilemap[i], Vec2{.x = x, .y = y });
	}

	//DrawSprite(player.sprite, player.position);
	player.Draw(this);

	for (int i = 0; i < enemies_alive; i++) {
		DrawSprite(enemies[i].sprite, enemies[i].position);
	}

	hud.Draw(this);

	// Probably want to make a way to draw to screen space not world space.
	font32.DrawString(this, "I can write normal text now.", Vec2{ 10.0f, 10.0f });
	font16.DrawString(this, "will convert to caps, and i have some symbols!", Vec2{10.0f, 60.0f});

	int seconds = static_cast<int>(elapsed_seconds);
	const int minutes = seconds / 60;
	seconds = seconds % 60;

	std::string seconds_string = "00";
	std::string minutes_string = "00";

	seconds_string[0] = std::to_string(seconds / 10)[0];
	seconds_string[1] = std::to_string(seconds % 10)[0];

	minutes_string[0] = std::to_string(minutes / 10)[0];
	minutes_string[1] = std::to_string(minutes % 10)[0];

	// I can work out text width very easily (32 * chars * 0.6) as 32 is the width of each char in my map, and 0.6 is the multiplier.
	font32.DrawString(this, minutes_string + ":" + seconds_string, Vec2{ 800.0f, 50.0f });

	if (active_level) active_level->Draw(this);

	window.present();
}

// Need to find better way to do this, this is very prone to error especially once we get to large numbers of resources.
static const char* GAME_IMAGE_PATH[GAME_IMAGE_COUNT] = {
	"resources/MainMenuHeader.png",
	"resources/ButtonBG.png",

	"resources/Player.png",
	"resources/PlayerWalk2.png",
	"resources/PlayerWalk3.png",
	"resources/Zombie.png",
	"resources/Archer.png",
	"resources/Tank.png",

	"resources/ItemBorder.png",

	"resources/AOEWeapon.png",

	"resources/0.png",
	"resources/1.png",
	"resources/2.png",
	"resources/3.png",
	"resources/4.png",
	"resources/5.png",
	"resources/6.png",
	"resources/7.png",
	"resources/8.png",
	"resources/9.png",
	"resources/10.png",

	"resources/MonoFont32pt.png",
	"resources/MonoFont16pt.png",
};

void Game::LoadAssets() {
	// TODO: CHECK IF ASSET EXISTS -> IF NOT JUST LOAD A BASE ONE WHICH IS CLEAR THAT IT'S NOT LOADED.
	for (int i = 0; i < GAME_IMAGE_COUNT; i++) {
		images[i].load(GAME_IMAGE_PATH[i]);
	}
}

void Game::DrawSprite(const Sprite& sprite, const Vec2& position) {
	const GamesEngineeringBase::Image* image = sprite.GetImage();
	if (!image) return;

	const Vec2 camera_pos = camera.position;

	// Want to round the position so movement is smoothed between pixels (I dont know if this actually has any effect).
	int round_x = static_cast<int>(round(position.x));
	int round_y = static_cast<int>(round(position.y));

	// The world position from screen 0, 0 -> w, h
	// This can be calculated once and cached before all our draw calls
	const int world_x_start = static_cast<int>(camera_pos.x) - (static_cast<int>(window_width) / 2);
	const int world_x_end = world_x_start + static_cast<int>(window_width);
	const int world_y_start = static_cast<int>(camera_pos.y) - (static_cast<int>(window_height) / 2);
	const int world_y_end = world_y_start + static_cast<int>(window_height);

	const int image_x_start = sprite.x_offset[0];
	const int image_x_end = sprite.x_offset[1] == -1 ? static_cast<int>(image->width) : sprite.x_offset[1];
	const int image_width = image_x_end - image_x_start;

	const int image_y_start = sprite.y_offset[0];
	const int image_y_end = sprite.y_offset[1] == -1 ? static_cast<int>(image->height) : sprite.y_offset[1];
	const int image_height = image_y_end - image_y_start;

	// start and end from 0 -> window_width
	const int image_visible_offset_x_start = max(round_x, world_x_start) - world_x_start;
	const int image_visible_offset_x_end = min(round_x + image_width, world_x_end) - world_x_start;

	const int image_visible_offset_y_start = max(round_y, world_y_start) - world_y_start;
	const int image_visible_offset_y_end = min(round_y + image_height, world_y_end) - world_y_start;

	const int image_x_offset = image_visible_offset_x_start - (round_x - world_x_start);
	const int image_y_offset = image_visible_offset_y_start - (round_y - world_y_start);
	int image_y = image_y_start + image_y_offset;

	for (int y = image_visible_offset_y_start; y < image_visible_offset_y_end; y++) {
		int image_x = image_x_start + image_x_offset - 1; // Start at - 1 so we can increment at start of loop.
		if (sprite.flip) image_x = image_x_end;
		for (int x = image_visible_offset_x_start; x < image_visible_offset_x_end; x++) {
			if (sprite.flip) image_x--;
			else image_x++;

			// If alpha 0, skip
			if (image->alphaAtUnchecked(image_x, image_y) == 0) continue;

			// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(y * window_width) + x]) continue;

			// Else draw and update depth buffer.
			const unsigned char* image_colour = image->atUnchecked(image_x, image_y);

			unsigned char final_colour[3] = {
				(sprite.modulation_colour[0] * image_colour[0]) / 255,
				(sprite.modulation_colour[1] * image_colour[1]) / 255,
				(sprite.modulation_colour[2] * image_colour[2]) / 255,
			};

			window.draw(x, y, &final_colour[0]);
			depth_buffer[(y * 1200) + x] = sprite.depth;
		}
		image_y++;
	}
}

void Game::DrawSpriteScreenSpace(const Sprite& sprite, const Vec2& position) {
	const GamesEngineeringBase::Image* image = sprite.GetImage();
	if (!image) return;

	const Vec2 camera_pos = camera.position;

	// Want to round the position so movement is smoothed between pixels (I dont know if this actually has any effect).
	int round_x = static_cast<int>(round(position.x));
	int round_y = static_cast<int>(round(position.y));

	const int image_x_start = sprite.x_offset[0];
	const int image_x_end = sprite.x_offset[1] == -1 ? static_cast<int>(image->width) : sprite.x_offset[1];
	const int image_width = image_x_end - image_x_start;

	const int image_y_start = sprite.y_offset[0];
	const int image_y_end = sprite.y_offset[1] == -1 ? static_cast<int>(image->height) : sprite.y_offset[1];
	const int image_height = image_y_end - image_y_start;

	// start and end from 0 -> window_width
	const int image_visible_offset_x_start = max(round_x, 0);
	const int image_visible_offset_x_end = min(round_x + image_width, window_width);

	const int image_visible_offset_y_start = max(round_y, 0);
	const int image_visible_offset_y_end = min(round_y + image_height, window_height);

	int image_y = image_y_start;
	for (int y = image_visible_offset_y_start; y < image_visible_offset_y_end; y++) {
		int image_x = image_x_start - 1; // Start at - 1 so we can increment at start of loop.
		if (sprite.flip) image_x = image_x_end;
		for (int x = image_visible_offset_x_start; x < image_visible_offset_x_end; x++) {
			if (sprite.flip) image_x--;
			else image_x++;

			// If alpha 0, skip
			if (image->alphaAtUnchecked(image_x, image_y) == 0) continue;

			// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(y * window_width) + x]) continue;

			// Else draw and update depth buffer.
			const unsigned char* image_colour = image->atUnchecked(image_x, image_y);

			unsigned char final_colour[3] = {
				(sprite.modulation_colour[0] * image_colour[0]) / 255,
				(sprite.modulation_colour[1] * image_colour[1]) / 255,
				(sprite.modulation_colour[2] * image_colour[2]) / 255,
			};

			window.draw(x, y, &final_colour[0]);
			depth_buffer[(y * 1200) + x] = sprite.depth;
		}
		image_y++;
	}
}
