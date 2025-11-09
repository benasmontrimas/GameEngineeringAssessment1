#include "Level.h"
#include "Game.h"
#include "AssetFilePaths.h"

#include <cassert>

#include "MainMenuLevel.h"

// NOTE: UI breaks when user resized the window manually, as this does not update the window width and height,
// and I don't want to edit the base header, I don't think there's a way to get the actual window width and height
// to calculate the actual button positions.

void Game::Init() {
	// Set a random seed at the start of the game.
	srand(static_cast<int>(time(nullptr)));

	window_width = 1200;
	window_height = 800;
	window.create(window_width, window_height, "Game");

	// Needs to be after we create window.
	LoadAssets();

	// Never gets freed but doesn't matter as when the game quits the program does as well.
	depth_buffer = static_cast<int*>(malloc(sizeof(int) * window_width * window_height));

	font32.Init(Font32Pt, 32, 32, 8);
	font16.Init(Font16Pt, 16, 16, 8);

	camera.Init();

	running = true;
	timer.reset();

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
	// We draw all the pixels on every frame, so this shouldn't be necessary.
	window.clear();

	// Clear Depth
	const unsigned int buffer_size = window_width * window_height;
	for (unsigned int i = 0; i < buffer_size; i++) {
		depth_buffer[i] = INT_MAX;
	}

	if (active_level_) active_level_->Draw(this);

	window.present();
}

void Game::LoadAssets() {
	// NOTE: Does not check if files exist, the load function crashes the program if it doesn't exist.
	for (int i = 0; i < GAME_IMAGE_COUNT; i++) {
		if (!images[i].load(game_image_path[i])) std::cout << "ERROR LOADING FILE\n";
	}
}

void Game::DrawRect(const Vec2& position, const Vec2& size, const int depth, const unsigned char r, const unsigned char g, const unsigned char b) {
	const auto [cam_x, cam_y] = camera.position;

	// Want to round the position so movement is smoothed between pixels (I don't know if this actually has any effect).
	const int round_x = static_cast<int>(round(position.x));
	const int round_y = static_cast<int>(round(position.y));

	// The world position from screen 0, 0 -> w, h
	// This can be calculated once and cached before all our draw calls
	const int world_x_start = static_cast<int>(cam_x) - (static_cast<int>(window_width) / 2);
	const int world_x_end = world_x_start + static_cast<int>(window_width);
	const int world_y_start = static_cast<int>(cam_y) - (static_cast<int>(window_height) / 2);
	const int world_y_end = world_y_start + static_cast<int>(window_height);

	const int image_width = static_cast<int>(size.x);
	const int image_height = static_cast<int>(size.y);

	// start and end from 0 -> window_width
	const int image_visible_offset_x_start = max(round_x, world_x_start) - world_x_start;
	const int image_visible_offset_x_end = min(round_x + image_width, world_x_end) - world_x_start;

	const int image_visible_offset_y_start = max(round_y, world_y_start) - world_y_start;
	const int image_visible_offset_y_end = min(round_y + image_height, world_y_end) - world_y_start;

	for (int y = image_visible_offset_y_start; y < image_visible_offset_y_end; y++) {
		for (int x = image_visible_offset_x_start; x < image_visible_offset_x_end; x++) {

			// If something is already drawn closer than this, skip.
			if (depth >= depth_buffer[(y * window_width) + x]) continue;

			// Else draw and update depth buffer.
			window.draw(x, y, r, g, b);
			depth_buffer[(y * window_width) + x] = depth;
		}
	}
}

void Game::DrawRectScreenSpace(const Vec2& position, const Vec2& size, int depth, unsigned char r, unsigned char g, unsigned char b) {
	for (int y = 0; y < static_cast<int>(size.y); y++) {
		const int screen_y_pos = static_cast<int>(position.y) + y;
		if (screen_y_pos < 0 or screen_y_pos > static_cast<int>(window_height)) continue;
		for (int x = 0; x < static_cast<int>(size.x); x++) {
			const int screen_x_pos = static_cast<int>(position.x) + x;
			if (screen_x_pos < 0 or screen_x_pos > static_cast<int>(window_width)) continue;

			if (depth >= depth_buffer[(screen_y_pos * window_width) + screen_x_pos]) continue;

			window.draw(screen_x_pos, screen_y_pos, r, g, b);
			depth_buffer[(screen_y_pos * window_width) + screen_x_pos] = depth;
		}
	}
}

void Game::DrawSprite(const Sprite& sprite, const Vec2& position) {
	const GamesEngineeringBase::Image* image = &images[sprite.GetImage()];
	if (!image) return;

	const auto [cam_x, cam_y] = camera.position;

	// Want to round the position so movement is smoothed between pixels (I dont know if this actually has any effect).
	const int round_x = static_cast<int>(round(position.x));
	const int round_y = static_cast<int>(round(position.y));

	// The world position from screen 0, 0 -> w, h
	// This can be calculated once and cached before all our draw calls
	const int world_x_start = static_cast<int>(cam_x) - (static_cast<int>(window_width) / 2);
	const int world_x_end = world_x_start + static_cast<int>(window_width);
	const int world_y_start = static_cast<int>(cam_y) - (static_cast<int>(window_height) / 2);
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

	int sign = +1;
	int image_x_starting_value = image_x_start + image_x_offset - 1;

	if (sprite.flip) {
		sign = -1;
		image_x_starting_value = image_x_end;
	}

	const unsigned char* image_data = image->data;

	for (int y = image_visible_offset_y_start; y < image_visible_offset_y_end; y++) {
		int image_x = image_x_starting_value;
		for (int x = image_visible_offset_x_start; x < image_visible_offset_x_end; x++) {
			image_x += sign;

			// If alpha 0, skip
			// I'm not calling alphaAtUnchecked because it's doing an extra if for channel size, but I assume channels is always 4.
			const unsigned char alpha = image_data[(((image_y * image->width) + image_x) * 4) + 3];
			//const unsigned char alpha = image->alphaAtUnchecked(image_x, image_y);
			if (alpha == 0) continue;

			//// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(y * window_width) + x]) continue;

			//// Else draw and update depth buffer.
			const unsigned char* image_colour = &image_data[((image_y * image->width) + image_x) * 4];

			unsigned char final_colour[3] = {
				static_cast<unsigned char>((sprite.modulation_colour[0] * image_colour[0]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[1] * image_colour[1]) / 255),
				static_cast<unsigned char>((sprite.modulation_colour[2] * image_colour[2]) / 255),
			};

			//// If transparent, blend colour with what currently in the colour buffer. This relies on correct ordering.
			if (alpha < 255)
			{
				const unsigned char* current_screen_colour = &window.backBuffer()[static_cast<size_t>((x + (y * window_width)) * 3)];

				const float alpha_as_percentage = static_cast<float>(alpha) / 255.0f;

				final_colour[0] = static_cast<unsigned char>(static_cast<float>(final_colour[0]) * alpha_as_percentage) + static_cast<unsigned char>(static_cast<float>(current_screen_colour[0]) * (1.0f - alpha_as_percentage));
				final_colour[1] = static_cast<unsigned char>(static_cast<float>(final_colour[1]) * alpha_as_percentage) + static_cast<unsigned char>(static_cast<float>(current_screen_colour[1]) * (1.0f - alpha_as_percentage));
				final_colour[2] = static_cast<unsigned char>(static_cast<float>(final_colour[2]) * alpha_as_percentage) + static_cast<unsigned char>(static_cast<float>(current_screen_colour[2]) * (1.0f - alpha_as_percentage));
			}

			window.draw(x, y, &final_colour[0]);
			depth_buffer[(y * window_width) + x] = sprite.depth;
		}
		image_y++;
	}
}

static Vec2 RotateVec2(const Vec2& vec, const float angle)
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
		rotation_matrix[0][0] * vec.x + rotation_matrix[0][1] * vec.y,
		rotation_matrix[1][0] * vec.x + rotation_matrix[1][1] * vec.y
	};

	return res;
}

// Not great, lots of duplicate code from the other draw sprite, but we need to do different maths and bounds checks.
// Could probably pull the major bits out into functions, ie getting screen space positions, and colour calculations.
// Could also just use this for all sprite drawing with default 0 for rotation, but will be slightly slower as we cant
// Check bounds as easy.
void Game::DrawSprite(const Sprite& sprite, const Vec2& position, const float rotation, const Vec2& around)
{
	const GamesEngineeringBase::Image* image = &images[sprite.GetImage()];
	if (!image) return;

	const auto [cam_x, cam_y] = camera.position;

	// Want to round the position so movement is smoothed between pixels (I don't know if this actually has any effect).
	const int round_x = static_cast<int>(round(position.x));
	const int round_y = static_cast<int>(round(position.y));

	// The world position from screen 0, 0 -> w, h
	// This can be calculated once and cached before all our draw calls
	const int world_x_start = static_cast<int>(cam_x) - (static_cast<int>(window_width) / 2);
	const int world_y_start = static_cast<int>(cam_y) - (static_cast<int>(window_height) / 2);

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

			// Need to offset by rotation point.
			Vec2 screen_position_vec = Vec2{ static_cast<float>(x), static_cast<float>(y) };
			screen_position_vec = screen_position_vec - (Vec2{ static_cast<float>(image_visible_offset_x_start), static_cast<float>(image_visible_offset_y_start) });
			screen_position_vec = screen_position_vec - around;

			// Then move back after rotation.
			Vec2 rotated_pos = RotateVec2(screen_position_vec, rotation);
			rotated_pos = rotated_pos + (Vec2{ static_cast<float>(image_visible_offset_x_start), static_cast<float>(image_visible_offset_y_start) });
			rotated_pos = rotated_pos + around;

			const int rotated_pos_x = static_cast<int>(rotated_pos.x);
			const int rotated_pos_y = static_cast<int>(rotated_pos.y);

			if (rotated_pos_x < 0 or rotated_pos_x >= static_cast<int>(window_width)) continue;
			if (rotated_pos_y < 0 or rotated_pos_y >= static_cast<int>(window_height)) continue;

			// If something is already drawn closer than this, skip.
			if (sprite.depth >= depth_buffer[(rotated_pos_y * window_width) + rotated_pos_x]) continue;

			window.draw(rotated_pos_x, rotated_pos_y, &final_colour[0]);
			depth_buffer[(rotated_pos_y * window_width) + rotated_pos_x] = sprite.depth;
		}
		image_y++;
	}
}

void Game::DrawSpriteScreenSpace(const Sprite& sprite, const Vec2& position) {
	const GamesEngineeringBase::Image* image = &images[sprite.GetImage()];
	if (!image) return;

	// Want to round the position so movement is smoothed between pixels (I don't know if this actually has any effect).
	const int round_x = static_cast<int>(round(position.x));
	const int round_y = static_cast<int>(round(position.y));

	const int image_x_start = sprite.x_offset[0];
	const int image_x_end = sprite.x_offset[1] == -1 ? static_cast<int>(image->width) : sprite.x_offset[1];
	const int image_width = image_x_end - image_x_start;

	const int image_y_start = sprite.y_offset[0];
	const int image_y_end = sprite.y_offset[1] == -1 ? static_cast<int>(image->height) : sprite.y_offset[1];
	const int image_height = image_y_end - image_y_start;

	// start and end from 0 -> window_width
	const int image_visible_offset_x_start = max(round_x, 0);
	const int image_visible_offset_x_end = min(round_x + image_width, static_cast<int>(window_width));

	const int image_visible_offset_y_start = max(round_y, 0);
	const int image_visible_offset_y_end = min(round_y + image_height, static_cast<int>(window_height));

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
			depth_buffer[(y * window_width) + x] = sprite.depth;
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
