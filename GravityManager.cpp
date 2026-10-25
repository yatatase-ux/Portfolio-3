#include "GravityManager.h"
#include "Shape.h"

GravityManager::GravityManager(const std::string stageNum, KeyAction& arg_key)
	:key(arg_key)
{
	gravity = GravityDir::Down;
	if (!stage.Load(stageNum))
	{
		// 必要になったら、タイトルに戻すなどの処理をここに書く
	}

	player = std::make_unique<Player>(key, stage);
}

GravityManager::~GravityManager()
{

}

void GravityManager::Input()
{
	if (key.Push(ONE))
	{
		gravity = GravityDir::Down;
		player->GravityChange(gravity);
		stage.SetGravityDir(gravity);
	}
	else if (key.Push(TWO))
	{
		gravity = GravityDir::Up;
		player->GravityChange(gravity);
		stage.SetGravityDir(gravity);
	}
	else if (key.Push(THREE))
	{
		gravity = GravityDir::Back;
		player->GravityChange(gravity);
		stage.SetGravityDir(gravity);
	}

	player->Input();
}

PlayerState GravityManager::Update()
{
	player->Update();
	camera.Follow(player->GetPos(), stage.GetStageSize());

	return  player->GetState();
}

void GravityManager::Draw()
{
	stage.Draw(camera);
	player->Draw(camera);

	DrawFormatString(20, 20, GetColor(244, 255, 255), "%d", static_cast<int>(gravity));

}