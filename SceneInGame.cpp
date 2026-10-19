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
	mGravity = std::make_unique<GravityManager>(stageName, manager->GetKey());
}

SCENE_UPDATE(SceneInGame)
{
	mGravity->Input();
	PlayerState pS = mGravity->Update();

	if (key->Push(TAB) || pS == PlayerState::Died)
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
	mGravity->Draw();
	DrawCenterString(200, 200, "InGame", GetColor(255, 255, 255), 30);
}