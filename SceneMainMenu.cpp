#include "SceneMainMenu.h"
#include "1-SceneInclude.h"
#include "SceneInGame.h"
#include <fstream>
#include <string>

int CountStages()
{
	int count = 0;

	while (true)
	{
		// 今の番号のファイル名を組み立てる(Load と同じルール)
		const std::string path = "StageInfo/CSV/Stage" + std::to_string(count) + "_side.csv";

		// 開けなければ、そこでステージは終わり
		std::ifstream file(path);
		if (!file.is_open()) break;

		count++;
	}

	return count;
}

SCENE_ENTER(SceneMainMenu)
{
	stageMax = CountStages();
}

SCENE_UPDATE(SceneMainMenu)
{
	if (key->Push(TAB))
	{
		manager->ChangeState(std::make_unique<SceneInGame>("Stage1"));
	}
}

SCENE_DRAW(SceneMainMenu)
{
	DrawCenterString(200, 200, "MainMenu", GetColor(255, 255, 255), 30);
	DrawCenterFormat(SCREEN_W / 2.0f, SCREEN_H / 2.0f, GetColor(255, 255, 0), 50, "ステージ数：%d", stageMax);
}

SCENE_EXIT(SceneMainMenu)
{

}
