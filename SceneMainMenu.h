#pragma once
#include "SceneBase.h"
#include <vector>

class SceneMainMenu : public SceneBase
{
	float baseX;
	int BG;

	int currentSelectStage;
	int stageMax;

	int noImage;
	std::vector<int> stageIcon;
	const float ICON_INTERVAL = 300.0f;
	const float ICON_MOVE_SPEED = 10.0f;

	bool isMove;

public:

	~SceneMainMenu();
	SCENE_CLASS(SceneMainMenu);

private:
	int CountStages();
};

