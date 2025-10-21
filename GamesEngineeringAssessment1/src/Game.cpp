#include "Game.h"

// Want to add some UI, health, what weapons are equiped. Enemies killed. Time.

void Game::Init() {
	// Set a random seed at the start of the game.
	srand(time(0));

	const int WINDOW_WIDTH = 1200;
	const int WINDOW_HEIGHT = 800;
	window.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	timer.reset();

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

	for (int i = 0; i < level_map.MAP_SIZE; i++) {
		DrawImage(level_map.tilemap[i], Vec2{ float(i % level_map.MAP_WIDTH) * float(level_map.tile_size), float(i / level_map.MAP_WIDTH) * float(level_map.tile_size) });
	}

	DrawImage(player.sprite, player.position);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		DrawImage(enemies[i].sprite, enemies[i].position);
	}

	hud.Draw(this);

	font32.DrawString(this, "!HELLO MY NAME IS BENAS (WOAH)", player.position);
	font16.DrawString(this, "!HELLO MY NAME IS BENAS (WOAH)", player.position + Vec2{ 0, 32.0f });

	window.present();
}

void Game::Update() {

	player.Update(this);
	camera.Update(delta_time);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		// Collision
		bool collided_with_player = enemies[i].collider.CheckCollision(player.collider);
		if (collided_with_player) {
			// Choose a spawn position
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

			enemies[i].position = enemies[i].position + (normalized_spawn_vector * abs(multiplier));
		}
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
	}
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
	// TODO: CHECK IF FONT EXISTS -> IF NOT JUST LOAD A BASE ONE WHICH IS CLEAR THAT ITS NOT LOADED.
	for (int i = 0; i < GAME_IMAGE_COUNT; i++) {
		images[i].load(GAME_IMAGE_PATH[i]);
	}
}

// TODO: Rewrite this. Its a mess, and theres a bug where flipped sprites on the right are not behaving as expected.
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