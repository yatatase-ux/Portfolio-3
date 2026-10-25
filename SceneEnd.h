#pragma once
#include "SceneBase.h"
#include "PlayerState.h"
#include "ScreenSize.h"

class SceneEnd :  public SceneBase
{
	PlayerState result;

public:
	SceneEnd(PlayerState r);

	SCENE_CLASS(SceneEnd);

};

