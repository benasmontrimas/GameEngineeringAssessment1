#pragma once
#include "Button.h"
#include "Sprite.h"

class Game;

class EndGameMenu
{
public:
	void Init(Game* game);
	void Update(Game* game);
	void Draw(Game* game);

public:
	int final_score;
	int time_survived;

private:
	Sprite background_;

	Button restart_button_;
	Button continue_button_;
};
