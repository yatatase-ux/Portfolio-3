#pragma once
#include <ctime>	
#include "DxLib.h"
#include <iostream>
#include <memory>

#include "SceneManager.h"
#include "SceneTitle.h"

class Game
{
private:

	SceneManager SM(std::make_unique<SceneTitle>());

public:

	Game();

	void GameLoop();

};