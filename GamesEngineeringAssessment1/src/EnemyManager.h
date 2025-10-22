#pragma once

#include "Enemy.h"

class EnemyManager {
public:
	void Init();
	void Update();
	
	int enemies_alive{};
	Enemy enemies[1000];
};