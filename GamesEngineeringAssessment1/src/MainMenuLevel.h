#pragma once
#include "Button.h"
#include "Level.h"
#include "Sprite.h"
#include "Map.h"

class MainMenuLevel : public Level {
public:
	void Init(Game* game) override;
	void Update(Game* game) override;
	void Draw(Game* game) override;
	void Shutdown(Game* game) override;

private:
	TileMap bg_;
	Sprite header_{};

	// TODO: Need a button class... This will get messy otherwise.
	Button play_button_;
	Button settings_button_;
	Button quit_button_;
};
