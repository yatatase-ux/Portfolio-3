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

	bool isMove;

public:

	~SceneMainMenu();
	SCENE_CLASS(SceneMainMenu);

private:
	int CountStages();
};

