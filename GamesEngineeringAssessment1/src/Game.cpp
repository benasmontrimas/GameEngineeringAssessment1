#include "Game.h"
#include <cassert>

#include "MainMenuLevel.h"

// NOTE: UI breaks when user resized the window manually, as this does not update the window width and height,
// and I don't want to edit the base header, I don't think there's a way to get the actual window width and height
// to calculate the actual button positions.

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

	font32.Init(&images[Font32Pt], 32, 32, 8);
	font16.Init(&images[Font16Pt], 16, 16, 8);

	running = true;
	timer.reset();

	camera.Init();

	// Set Main Menu as the starting level.
	active_level_ = new MainMenuLevel;
	active_level_->Init(this);
}

void Game::Run() {
	float count = 0.0f;
	int frame_count = 0;

	while (running) {
		delta_time = timer.dt();
		game_time = delta_time * game_time_multiplier;

		count += delta_time;
		frame_count++;
		if (count >= 1.0f) {
			fps = frame_count;
			count = 0.0f;
			frame_count = 0;
		}

		window.checkInput();

		if (next_level_) SwitchLevel();

		Update();
		Render();
	}
}

void Game::Update() {
	if (active_level_) active_level_->Update(this);
	camera.Update(game_time);
}

void Game::Render() {
	// Once we have a background we should be able to ignore clearing, as all the pixels will
	// get over drawn anyway.
	window.clear();

	// Clear Depth
	const unsigned int buffer_size = window_width * window_height;
	for (unsigned int i = 0; i < buffer_size; i++) {
		depth_buffer[i] = INT_MAX;
	}

	if (active_level_) active_level_->Draw(this);

	window.present();
}

// Need to find better way to do this, this is very prone to error especially once we get to large numbers of resources.
static constexpr char const* GAME_IMAGE_PATH[GAME_IMAGE_COUNT] = {
	"resources/MainMenuBG.png",
	"resources/MainMenuHeader.png",
	"resources/ButtonBG.png",
	"resources/PopupMenuBG.png",

	"resources/PlayerIdle1.png",
	"resources/PlayerIdle2.png",
	"resources/PlayerIdle3.png",
	"resources/PlayerIdle4.png",
	"resources/PlayerIdle5.png",
	"resources/PlayerIdle6.png",
	"resources/PlayerIdle7.png",

	"resources/PlayerWalk1.png",
	"resources/PlayerWalk2.png",
	"resources/PlayerWalk3.png",
	"resources/PlayerWalk4.png",
	"resources/PlayerWalk5.png",
	"resources/PlayerWalk6.png",
	"resources/PlayerWalk7.png",
	"resources/PlayerWalk8.png",
	"resources/PlayerWalk9.png",

	"resources/Player.png",

	"resources/SwordsmanWalk1.png",
	"resources/SwordsmanWalk2.png",
	"resources/SwordsmanWalk3.png",
	"resources/SwordsmanWalk4.png",
	"resources/SwordsmanWalk5.png",
	"resources/SwordsmanWalk6.png",
	"resources/SwordsmanWalk7.png",
	"resources/SwordsmanWalk8.png",
	"resources/SwordsmanWalk9.png",
	"resources/SwordsmanWalk10.png",

	"resources/SwordsmanAttack1.png",
	"resources/SwordsmanAttack2.png",
	"resources/SwordsmanAttack3.png",
	"resources/SwordsmanAttack4.png",
	"resources/SwordsmanAttack5.png",
	"resources/SwordsmanAttack6.png",
	"resources/SwordsmanAttack7.png",
	"resources/SwordsmanAttack8.png",
	"resources/SwordsmanAttack9.png",
	"resources/SwordsmanAttack10.png",
	"resources/SwordsmanAttack11.png",
	"resources/SwordsmanAttack12.png",
	"resources/SwordsmanAttack13.png",
	"resources/SwordsmanAttack14.png",
	"resources/SwordsmanAttack15.png",

	"resources/ArcherWalk1.png",
	"resources/ArcherWalk2.png",
	"resources/ArcherWalk3.png",
	"resources/ArcherWalk4.png",
	"resources/ArcherWalk5.png",
	"resources/ArcherWalk6.png",
	"resources/ArcherWalk7.png",
	"resources/ArcherWalk8.png",
	"resources/ArcherWalk9.png",
	"resources/ArcherWalk10.png",

	"resources/Tank.png",

	"resources/ItemBorder.png",

	"resources/AOEWeapon.png",

	"resources/SwordThrow1.png",
	"resources/SwordThrow2.png",
	"resources/SwordThrow3.png",
	"resources/SwordThrow4.png",
	"resources/SwordThrow5.png",
	"resources/SwordThrow6.png",
	"resources/SwordThrow7.png",
	"resources/SwordThrow8.png",

	"resources/Water1.png",
	"resources/Water2.png",
	"resources/Water3.png",
	"resources/Water4.png",
	"resources/Water5.png",
	"resources/Water6.png",
	"resources/Water7.png",
	"resources/Water8.png",

	"resources/Grass1.png",

	"resources/Path1.png",

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
		if (!images[i].load(GAME_IMAGE_PATH[i])) std::cout << "ERROR LOADING FILE\n";
	}
}

void Game::DrawSprite(const Sprite& sprite, const Vec2& position) {
	const GamesEngineeringBase::Image* image = sprite.GetImage();
	if (!image) return;

	const Vec2 camera_pos = camera.position;

	// Want to round the position so movement is smoothed between pixels (I dont know if this actually has any effect).
	const int round_x = static_cast<int>(round(position.x));
	const int round_y = static_cast<int>(round(position.y));

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
				static_cast<unsigned char>((sprite.modulation_colour[0] * image_colour[0]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[1] * image_colour[1]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[2] * image_colour[2]) / 255),
			};

			window.draw(x, y, &final_colour[0]);
			depth_buffer[(y * 1200) + x] = sprite.depth;
		}
		image_y++;
	}
}

static Vec2 RotateVec2(Vec2& vec, const float angle)
{
	Vec2 rotation_matrix[2] = {
		{
			.x = cos(angle), .y = -sin(angle)
		},
		{
			.x = sin(angle), .y = cos(angle)
		}
	};

	const Vec2 res = {
		rotation_matrix[0][0] * vec[0] + rotation_matrix[0][1] * vec[1],
		rotation_matrix[1][0] * vec[0] + rotation_matrix[1][1] * vec[1]
	};

	return res;
}

// Not great, lots of duplicate code from the other draw sprite, but we need to do different maths and bounds checks.
// Could probably pull the major bits out into functions, ie getting screen space positions, and colour calculations.
// Could also just use this for all sprite drawing with default 0 for rotation, but will be slightly slower as we cant
// Check bounds as easy.
void Game::DrawSprite(const Sprite& sprite, const Vec2& position, const float rotation, const Vec2& around)
{
	const GamesEngineeringBase::Image* image = sprite.GetImage();
	if (!image) return;

	const Vec2 camera_pos = camera.position;

	// Want to round the position so movement is smoothed between pixels (I don't know if this actually has any effect).
	const int round_x = static_cast<int>(round(position.x));
	const int round_y = static_cast<int>(round(position.y));

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
	const int image_visible_offset_x_start = round_x - world_x_start;
	const int image_visible_offset_x_end = round_x + image_width - world_x_start;

	const int image_visible_offset_y_start = round_y - world_y_start;
	const int image_visible_offset_y_end = round_y + image_height - world_y_start;

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

			// Else draw and update depth buffer.
			const unsigned char* image_colour = image->atUnchecked(image_x, image_y);

			unsigned char final_colour[3] = {
				static_cast<unsigned char>((sprite.modulation_colour[0] * image_colour[0]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[1] * image_colour[1]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[2] * image_colour[2]) / 255),
			};

			Vec2 screen_position_vec = Vec2(float(x), float(y)) - (Vec2{ float(image_visible_offset_x_start), float(image_visible_offset_y_start)}) - around;
			Vec2 rotated_pos = RotateVec2(screen_position_vec, rotation) + (Vec2{ float(image_visible_offset_x_start), float(image_visible_offset_y_start) }) + around;

			if (rotated_pos.x < 0 or rotated_pos.x >= window_width) continue;
			if (rotated_pos.y < 0 or rotated_pos.y >= window_height) continue;

			// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(int(rotated_pos.y) * window_width) + int(rotated_pos.x)]) continue;

			window.draw(int(rotated_pos.x), int(rotated_pos.y), &final_colour[0]);
			depth_buffer[(int(rotated_pos.y) * 1200) + int(rotated_pos.x)] = sprite.depth;
		}
		image_y++;
	}
}

void Game::DrawSpriteScreenSpace(const Sprite& sprite, const Vec2& position) {
	const GamesEngineeringBase::Image* image = sprite.GetImage();
	if (!image) return;

	const Vec2 camera_pos = camera.position;

	// Want to round the position so movement is smoothed between pixels (I don't know if this actually has any effect).
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
				static_cast<unsigned char>((sprite.modulation_colour[0] * image_colour[0]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[1] * image_colour[1]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[2] * image_colour[2]) / 255),
			};

			window.draw(x, y, &final_colour[0]);
			depth_buffer[(y * 1200) + x] = sprite.depth;
		}
		image_y++;
	}
}

void Game::SwitchLevel()
{
	delete active_level_;
	active_level_ = next_level_;
	next_level_ = nullptr;
	active_level_->Init(this);
}

bool Game::SetNextLevel(Level* level)
{
	if (next_level_) return false;
	next_level_ = level;
	return true;
}

Level* Game::GetLevel() const
{
	return active_level_;
}
