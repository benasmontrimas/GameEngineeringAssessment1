#pragma once

class Game;

class Level {
public:
	virtual void Init(Game* game) = 0;
	virtual void Update(Game* game) = 0;
	virtual void Draw(Game* game) = 0;
	virtual void Shutdown(Game* game) = 0;
};