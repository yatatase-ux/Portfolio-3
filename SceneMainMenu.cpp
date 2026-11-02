#include "SceneMainMenu.h"
#include "1-SceneInclude.h"
#include "SceneInGame.h"
#include <fstream>
#include <string>
#include <algorithm>

SceneMainMenu::~SceneMainMenu()
{
	DeleteGraph(BG);
}

int SceneMainMenu::CountStages()
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
	BG = LoadGraph("Data/image/BG/MainMenuBG.jpg");

	baseX = SCREEN_W / 2.0f;

	currentSelectStage = 0;
	stageMax = CountStages();

	// メニューに戻ってきたときに、前のアイコンが残らないようにする
	stageIcon.clear();

	// 画像がないときの代わりの画像(1回だけ読み込んで使い回す)
	noImage = LoadGraph("Data/image/StageIcon/NoImage.png");

	isMove = false;

	for (int i = 0; i < stageMax; i++)
	{
		// 数字は to_string で文字列にしてからつなげる
		const std::string path = "Data/image/StageIcon/Stage" + std::to_string(i) + "_Icon.png";

		// 読み込みに失敗すると -1 が返る(ifstream で開いて確かめる必要はない)
		int handle = LoadGraph(path.c_str());
		if (handle == -1)
		{
			// 画像がないステージは、代わりの画像を使う
			handle = noImage;
		}

		// 1ステージにつき、必ず1つだけ追加する(番号とアイコンをずらさない)
		stageIcon.push_back(handle);
	}
}

SCENE_UPDATE(SceneMainMenu)
{
	int beforeSelect = currentSelectStage;
	if (key->Push(RIGHT))
	{
		currentSelectStage++;
		isMove = true;
	}
	if (key->Push(LEFT))
	{
		currentSelectStage--;
		isMove = true;
	}
	currentSelectStage = std::clamp(currentSelectStage, 0, stageMax - 1);
	// 目標の位置(選んでいるステージが画面の中央に来る baseX)
	const float targetX = SCREEN_W / 2.0f - 300.0f * currentSelectStage;

	// 目標までの差を、1フレームで動ける量(±20)に収めてから足す
	// → 遠くても近くても同じ速さで動き、目標でぴったり止まる
	const float diffX = targetX - baseX;
	baseX += std::clamp(diffX, -20.0f, 20.0f);

	// 目標に着いていなければ「移動中」
	isMove = (baseX != targetX);

	if (!isMove)
	{
		if (key->Push(TAB))
		{
			const std::string stageNum = "Stage" + std::to_string(currentSelectStage);
			manager->ChangeState(std::make_unique<SceneInGame>(stageNum));
		}
	}
}

SCENE_DRAW(SceneMainMenu)
{
	DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, BG, TRUE);

	for (int i = 0; i < stageMax; i++)
	{
		float x = baseX + 300 * i;
		float y = SCREEN_H / 2.0f;
		float dis = abs(SCREEN_W / 2.0f - x) / 500.0f;
		float rate = 1.0f - dis;
		rate = std::clamp(rate, 0.5f, 1.0f);
		DrawRotaGraph(x, y, rate, 0, stageIcon[i], TRUE);
	}
}

SCENE_EXIT(SceneMainMenu)
{
	
}
