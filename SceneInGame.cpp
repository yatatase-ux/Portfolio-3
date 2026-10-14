#include "SceneInGame.h"
#include "1-SceneInclude.h"
#include "SceneEnd.h"

// 引数付きのコンストラクタは SCENE_CONST マクロが使えないので普通に書く
// 受け取ったステージ名をメンバに保存するだけ(読み込みは Enter で行う)
SceneInGame::SceneInGame(const std::string& arg_stageName)
	: stageName(arg_stageName)
{
}

SCENE_ENTER(SceneInGame)
{
	player = std::make_unique<Player>(manager->GetKey(), stage);

	// ステージの読み込み
	// 失敗したときはデバッグ表示が出るので、まずはそのまま進める
	if (!stage.Load(stageName))
	{
		// 必要になったら、タイトルに戻すなどの処理をここに書く
	}
}

SCENE_UPDATE(SceneInGame)
{
	player->Input();
	player->Update();
	if (key->Push(TAB))
	{
		manager->ChangeState(std::make_unique<SceneEnd>());
		return;
	}
}

SCENE_EXIT(SceneInGame)
{

}

SCENE_DRAW(SceneInGame)
{
	stage.Draw();
	DrawCenterString(200, 200, "InGame", GetColor(255, 255, 255), 30);
	player->Draw();
}