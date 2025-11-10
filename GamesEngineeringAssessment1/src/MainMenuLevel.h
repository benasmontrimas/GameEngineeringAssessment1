#pragma once
#include "Button.h"
#include "Level.h"
#include "Sprite.h"
#include "TileMap.h"
#include "GameLevel.h"

class MainMenuLevel : public Level {
public:
	void Init(Game* game) override;
	void Update(Game* game) override;
	void Draw(Game* game) override;

private:
	static void LoadLevel(Game* game);

public:
	GameDifficulty difficulty{ Easy };

private:
	TileMap bg_{};
	Sprite header_{};

	Button load_button_;
	Button play_infinite_button_;
	Button play_fixed_button_;
	Button difficulty_button_;
	Button quit_button_;

};
