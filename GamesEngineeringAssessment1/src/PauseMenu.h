#pragma once
#include "Button.h"

class Game;

class PauseMenu
{
public:
	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game) const;

	[[nodiscard]] bool IsContinue() const;

private:
	Sprite background_;
	Button continue_button_;
	Button save_button_;
	Button quit_button_;
};
