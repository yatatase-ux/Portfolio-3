#pragma once
#include <memory>
#include "SceneManager.h"

class GameLoop
{
	std::unique_ptr<SceneManager> SM;

public:
	GameLoop();

	void Loop();
};