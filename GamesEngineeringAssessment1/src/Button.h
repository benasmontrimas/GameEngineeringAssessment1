#pragma once
#include "Sprite.h"
#include <string>

#include "CustomBase.h"

class Game;

class Button {
public:
	void Init(Game* game, const std::string& text);
	void Update(const Game* game);
	void Draw(Game* game) const;

public:
	static constexpr unsigned int width = 200;
	static constexpr unsigned int height = 50;

	Vec2 position{};

private:
	Sprite background_;
	std::string text_;

	bool is_hovered_{false};
};