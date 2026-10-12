#include "SceneInGame.h"
#include "1-SceneInclude.h"
#include "SceneEnd.h"

SCENE_ENTER(SceneInGame)
{
	player = std::make_unique<Player>(manager->GetKey());
}

SCENE_UPDATE(SceneInGame)
{

	Draw();
	Sound();


	if (key->Push(SPACE))
	{
		ChangeScene(manager);
	}
}

SCENE_EXIT(SceneInGame)
{

}

SCENE_DRAW(SceneInGame)
{
	DrawCenterString(200, 200, "InGame", GetColor(255, 255, 255), 30);
	player->Draw();
}

SCENE_SOUND(SceneInGame)
{

}

SCENE_CHANGE(SceneInGame)
{
	manager->ChangeState(std::make_unique<SceneEnd>());
}