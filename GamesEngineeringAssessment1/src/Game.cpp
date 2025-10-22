#include "Game.h"
#include <cassert>

// Want to add some UI, health, what weapons are equiped. Enemies killed. Time.

void Game::Init() {
	// Set a random seed at the start of the game.
	srand(time(0));

	const int WINDOW_WIDTH = 1200;
	const int WINDOW_HEIGHT = 800;
	window.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	window_width = window.getWidth();
	window_height = window.getHeight();

	// Needs to be after we create window.
	LoadAssets();

	// Create Depth Array
	depth_buffer = static_cast<int*>(malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT));

	level_map.Init(this);
	player.Init(this);
	camera.Init();
	hud.Init(this);

	font32.Init(&images[Font32pt], 32, 32, 8);
	font16.Init(&images[Font16pt], 16, 16, 8);

	camera.SetFollow(&player.position);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].Init(this, Zombie);
	}

	running = true;
	timer.reset();
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
	int rand_x = rand() % 2000;
	int rand_y = rand() % 2000;
	spawn_vector.x = float(rand_x - 1000) / 1000.0f;
	spawn_vector.y = float(rand_y - 1000) / 1000.0f;

	Vec2 normalized_spawn_vector = NormalizeVec2(spawn_vector);
	float multiplier = 0.0f;
	if (abs(window_width / normalized_spawn_vector.x) < abs(window_height / normalized_spawn_vector.y)) {
		// sides closer
		multiplier = float(window_width) / normalized_spawn_vector.x;
	}
	else {
		// top/bottom closer
		multiplier = float(window_height) / normalized_spawn_vector.y;
	}

	enemies[enemies_alive].position = camera.position + (normalized_spawn_vector * abs(multiplier));
	enemies_alive++;
}

void Game::Update() {

	if (last_spawn != int(elapsed_seconds / spawn_cooldown)) {
		int number_of_enemies_to_spawn = MAX_ENEMIES / (level_duration / elapsed_seconds);
		number_of_enemies_to_spawn = min(number_of_enemies_to_spawn + 10, MAX_ENEMIES - enemies_alive);

		for (int i = 0; i < number_of_enemies_to_spawn; i++) {
			SpawnEnemy();
		}

		std::cout << number_of_enemies_to_spawn << "\n";

		last_spawn = int(elapsed_seconds / spawn_cooldown);
	}

	player.Update(this);
	camera.Update(delta_time);
	level_map.Update(this);

	int enemies_to_remove = 0;
	int enemy_to_remove[MAX_ENEMIES];

	for (int i = 0; i < enemies_alive; i++) {
		// Collision
		bool collided_with_player = enemies[i].collider.CheckCollision(player.collider);
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

	for (int i = 0; i < level_map.tiles_in_use; i++) {
		float x = float(level_map.render_x) + (float(i % level_map.tiles_in_row) * float(level_map.tile_size));
		float y = float(level_map.render_y) + (float(i / level_map.tiles_in_row) * float(level_map.tile_size));
		DrawSprite(level_map.tilemap[i], Vec2{ x, y });
	}

	//DrawSprite(player.sprite, player.position);
	player.Draw(this);

	for (int i = 0; i < enemies_alive; i++) {
		DrawSprite(enemies[i].sprite, enemies[i].position);
	}

	hud.Draw(this);

	// Probably want to make a way to draw to screen space not world space.
	font32.DrawString(this, "I can write normal text now.", camera.position + Vec2{ 50.f - (window_width / 2.0f), 50.0f - (window_height / 2.0f) });
	font16.DrawString(this, "will convert to caps, and i have some symbols!", camera.position + Vec2{ 50.f - (window_width / 2.0f), 100.0f - (window_height / 2.0f) });

	int seconds = static_cast<int>(elapsed_seconds);
	int minutes = seconds / 60;
	seconds = seconds % 60;

	std::string seconds_string = "00";
	std::string minutes_string = "00";

	seconds_string[0] = std::to_string(seconds / 10)[0];
	seconds_string[1] = std::to_string(seconds % 10)[0];

	minutes_string[0] = std::to_string(minutes / 10)[0];
	minutes_string[1] = std::to_string(minutes % 10)[0];

	// I can work out text width very easily (32 * chars * 0.6) as 32 is the width of each char in my map, and 0.6 is the multiplier.
	font32.DrawString(this, minutes_string + ":" + seconds_string, camera.position + Vec2{ -(96.f / 2.0f), 40.0f - (window_height / 2.0f) });

	window.present();
}

// Need to find better way to do this, this is very prone to error especially once we get to large numbers of resources.
const char* GAME_IMAGE_PATH[GAME_IMAGE_COUNT] = {
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
	// TODO: CHECK IF ASSET EXISTS -> IF NOT JUST LOAD A BASE ONE WHICH IS CLEAR THAT ITS NOT LOADED.
	for (int i = 0; i < GAME_IMAGE_COUNT; i++) {
		images[i].load(GAME_IMAGE_PATH[i]);
	}
}

void Game::DrawSprite(const Sprite& sprite, const Vec2& position) {
	GamesEngineeringBase::Image* image = sprite.GetImage();
	if (!image) return;

	Vec2 camera_pos = camera.position;

	// Want to round the position so movement is smoothed between pixels (I dont know if this actually has any effect).
	int x = static_cast<int>(round(position.x));
	int y = static_cast<int>(round(position.y));

	// The world position from screen 0, 0 -> w, h
	// This can be calculated once and cached before all our draw calls
	int world_x_start = static_cast<int>(camera_pos.x) - (window_width / 2);
	int world_x_end = world_x_start + window_width;
	int world_y_start = static_cast<int>(camera_pos.y) - (window_height / 2);
	int world_y_end = world_y_start + window_height;

	int image_x_start = sprite.x_offset[0];
	int image_x_end = sprite.x_offset[1] == -1 ? image->width : sprite.x_offset[1];
	int image_width = image_x_end - image_x_start;

	int image_y_start = sprite.y_offset[0];
	int image_y_end = sprite.y_offset[1] == -1 ? image->height : sprite.y_offset[1];
	int image_height = image_y_end - image_y_start;

	// start and end from 0 -> window_width
	int image_visible_offset_x_start = max(x, world_x_start) - world_x_start;
	int image_visible_offset_x_end = min(x + image_width, world_x_end) - world_x_start;
	
	int image_visible_offset_y_start = max(y, world_y_start) - world_y_start;
	int image_visible_offset_y_end = min(y + image_height, world_y_end) - world_y_start;

	int image_x_offset = image_visible_offset_x_start - (x - world_x_start);
	int image_y_offset = image_visible_offset_y_start - (y - world_y_start);
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
			unsigned char* image_colour = image->atUnchecked(image_x, image_y);

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

// DEPRECATED
void Game::DrawImage(const Sprite& sprite, const Vec2& position) {
	GamesEngineeringBase::Image* image = sprite.GetImage();
	if (image == nullptr) return;

	int x_start = sprite.x_offset[0];
	int x_end = sprite.x_offset[1];
	if (x_end == -1) x_end = image->width;

	int y_start = sprite.y_offset[0];
	int y_end = sprite.y_offset[1];
	if (y_end == -1) y_end = image->height;

	// I need image xy, screen xy, world xy
	int world_x_pos_at_0_0 = camera.position.x - (window_width / 2);
	int world_y_pos_at_0_0 = camera.position.y - (window_height / 2);
	
	// Clamp our values to screen limits.
	int image_x_start = max(static_cast<int>(position.x), world_x_pos_at_0_0);
	int image_y_start = max(static_cast<int>(position.y), world_y_pos_at_0_0);

	int image_x_end = min(static_cast<int>(position.x) + (x_end - x_start), world_x_pos_at_0_0 + window_width);
	int image_y_end = min(static_cast<int>(position.y) + (y_end - y_start), world_y_pos_at_0_0 + window_height);

	// This is the slow down for rendering enemies, if we sort enemies by their depth, we can
	// remove loads of overdraw.
	for (int image_y = image_y_start; image_y < image_y_end; image_y++) {
		int screen_y = image_y - world_y_pos_at_0_0;
		int image_coord_y = (image_y - position.y) + y_start;

		for (int image_x = image_x_start; image_x < image_x_end; image_x++) {
			int x = image_x;
			int screen_x = image_x - world_x_pos_at_0_0;

			if (sprite.flip) {
				x = image_x_start + (image_x_end - image_x);
			}

			int image_coord_x = (x - position.x) + x_start;
			// If transparent skip
			// TODO: Add semi transparent objects.
			// - Would probably want a seperate queue for transparent objects, and render them after all
			//	 opaque sprites. Then we can just do some basic colour calc on whatever colour is on top.

			// If alpha is 0, skip
			if (image->alphaAtUnchecked(image_coord_x, image_coord_y) == 0) continue;

			// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(screen_y * window_width) + screen_x]) continue;

			window.draw(screen_x, screen_y, image->atUnchecked(image_coord_x, image_coord_y));
			depth_buffer[(screen_y * 1200) + screen_x] = sprite.depth;

		}
	}
}