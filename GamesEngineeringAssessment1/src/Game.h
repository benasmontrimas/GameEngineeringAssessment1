#pragma once

#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Sprite.h"
#include "FontMap.h"
#include "Level.h"
#include "GameImages.h"

class Game {
public:
	Game() = default;
	~Game() = default;

	// Don't allow any copying or moving.
	Game(Game& g) = delete;
	Game(Game&& g) = delete;
	Game& operator=(Game& other) = delete;
	Game& operator=(Game&& other) = delete;

	void Init();
	void Run();

	void DrawRect(const Vec2& position, const Vec2& size, int depth, unsigned char r, unsigned char g, unsigned char b);
	void DrawRectScreenSpace(const Vec2& position, const Vec2& size, int depth, unsigned char r, unsigned char g, unsigned char b);
	void DrawSprite(const Sprite& sprite, const Vec2& position);
	void DrawSprite(const Sprite& sprite, const Vec2& position, float rotation, const Vec2& around);
	void DrawSpriteScreenSpace(const Sprite& sprite, const Vec2& position);

	bool SetNextLevel(Level* level);

	[[nodiscard]] Level* GetLevel() const;

private:
	void Render();
	void Update();
	void LoadAssets();
	void SwitchLevel();

private:
	Level* active_level_;
	Level* next_level_;

public:
	// Systems
	GamesEngineeringBase::Window window{};
	GamesEngineeringBase::Image images[GAME_IMAGE_COUNT];
	GamesEngineeringBase::Timer timer{};

	Camera camera{};

	FontMap font32{};
	FontMap font16{};

	// Other
	unsigned int window_width{};
	unsigned int window_height{};
	int* depth_buffer{};

	// Need to separate out game time and real time for pausing.
	float delta_time{};
	float game_time_multiplier = 1.0f;
	float game_time{};

	int fps{0};

	bool running = false;
};