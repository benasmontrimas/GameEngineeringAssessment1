#pragma once
#include "Sprite.h"
#include <string>

#include "Vec2.h"

class Game;

class Button {
public:
	void Init(Game* game);
	void Update(const Game* game);
	void Draw(Game* game, const std::string& text) const;

	// Returns true if the button was pressed this frame.
	[[nodiscard]] bool IsPressed() const;

public:
	static constexpr unsigned int width = 200;
	static constexpr unsigned int height = 50;

	Vec2 position{};

private:
	Sprite background_{};

	bool is_hovered_{ false };
	bool is_pressed_{ false };
	bool is_pressed_last_frame_{ false };
};