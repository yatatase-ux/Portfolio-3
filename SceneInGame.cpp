#include "SceneInGame.h"
#include "SceneInclude.h"
#include "SceneMainMenu.h"
#include "SceneEnd.h"

// 引数付きのコンストラクタは SCENE_CONST マクロが使えないので普通に書く
// 受け取ったステージ名をメンバに保存するだけ(読み込みは Enter で行う)
SceneInGame::SceneInGame(const std::string& arg_stageName)
	: stageName(arg_stageName)
{
	
}

SCENE_ENTER(SceneInGame)
{
	// ---- ステージ ----
	stage = std::make_unique<Stage>();
	if (!stage->Load(stageName))
	{
		// 読み込めなかったら、メニューに戻る(空のステージで遊ばないように)
		manager->ChangeState(std::make_unique<SceneMainMenu>());
		return;
	}
	// ---- 重力操作 ----
	mGravity = std::make_unique<GravityManager>(stageName, manager->GetKey(), stage->GetStartGravity());
	stage->SetGravityDir(mGravity->GetCurrentGravity());
	// ---- カメラ ----
	camera = std::make_unique<Camera>();
	// ---- プレイヤー ----
	player = std::make_unique<Player>(manager->GetKey(), *stage, mGravity->GetCurrentGravity());

	// ---- InGameの変数 ----
	state = InGameState::Playing;

}

SCENE_UPDATE(SceneInGame)
{
	bool changeGra = mGravity->Input();
	if (changeGra)
	{
		player->GravityChange(mGravity->GetCurrentGravity());
		stage->SetGravityDir(mGravity->GetCurrentGravity());
	}
	player->Input();

	mGravity->Update();
	player->Update();
	camera->Follow(player->GetPos(), stage->GetStageSize());

	PlayerState pS = player->GetState();

	if (key->Push(TAB) || player->GetState() == PlayerState::Dead)
	{
		manager->ChangeState(std::make_unique<SceneEnd>(pS));
		return;
	}
	if (player->GetState() == PlayerState::Clear)
	{
		manager->ChangeState(std::make_unique<SceneEnd>(pS));
		return;
	}

	//switch(state)
	//{
	//case InGameState::Playing:
	//{
	//	break;
	//}

	//case InGameState::Clearing:
	//{
	//	break;
	//}

	//case InGameState::Missing:
	//{
	//	break;
	//}
	//}
}

SCENE_EXIT(SceneInGame)
{

}

SCENE_DRAW(SceneInGame)
{
	stage->Draw(*camera);
	player->Draw(*camera);
	mGravity->Draw();
	DrawCenterString(200, 200, "InGame", GetColor(255, 255, 255), 30);
}