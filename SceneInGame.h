#pragma once
#include "SceneBase.h"
#include "Player.h"
#include <memory>

class SceneInGame : public SceneBase
{
	std::unique_ptr<Player> player;

public:
	SCENE_CLASS(SceneInGame);

};

