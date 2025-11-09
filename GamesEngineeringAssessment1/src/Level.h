#pragma once

class Game;

class Level {
public:
	Level() = default;
	virtual ~Level() = default;

	// No copy or moving.
	Level(Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level& other) = delete;
	Level& operator=(Level&& other) = delete;

	virtual void Init(Game* game) = 0;
	virtual void Update(Game* game) = 0;
	virtual void Draw(Game* game) = 0;
};