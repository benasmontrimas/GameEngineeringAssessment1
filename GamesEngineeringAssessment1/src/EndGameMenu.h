#pragma once
#include "Button.h"
#include "Sprite.h"

class Game;

class EndGameMenu
{
public:
	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game) const;

public:
	int final_score;
	int time_survived;

private:
	Sprite background_{};

	Button restart_button_;
	Button main_menu_button_;
};
