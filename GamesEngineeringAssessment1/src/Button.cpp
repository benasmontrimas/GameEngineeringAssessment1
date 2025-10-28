#include "Button.h"
#include "Game.h"

void Button::Init(Game* game, const std::string& text) {
	text_ = text;

	background_.Init(1);
	background_.images[0] = &game->images[ButtonBG];
	background_.depth = INT_MIN + 1;
}

void Button::Update(const Game* game)
{
	const GamesEngineeringBase::Window& window = game->window;

	// Probably want to cache these in the game class.
	const int mouse_x = window.getMouseInWindowX();
	const int mouse_y = window.getMouseInWindowY();

	if (mouse_x < static_cast<int>(position.x) or mouse_x > static_cast<int>(position.x) + static_cast<int>(width) or
		mouse_y < static_cast<int>(position.y) or mouse_y > static_cast<int>(position.y) + static_cast<int>(height))
	{
		background_.modulation_colour[0] = 0;
		is_hovered_ = false;
	}
	else
	{
		background_.modulation_colour[0] = 255;
		is_hovered_ = true;
	}

	if (is_hovered_ and window.mouseButtonPressed(GamesEngineeringBase::MouseLeft))
	{
		// Need to add callbacks, make this call some function;
		std::cout << text_ << ", BUTTON CLICKED!\n";
	} 
}

void Button::Draw(Game* game) const
{
	game->DrawSpriteScreenSpace(background_, position);
	game->font32.DrawString(game, text_, position + Vec2{10.0f, 10.0f});
}
