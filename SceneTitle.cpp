#include "SceneTitle.h"
#include "1-SceneInclude.h"
#include "SceneInGame.h"

SCENE_ENTER(SceneTitle)
{

}

SCENE_UPDATE(SceneTitle)
{
	if (key->Push(SPACE))
	{
		manager->ChangeState(std::make_unique<SceneInGame>("Stage0"));
	}
}

SCENE_EXIT(SceneTitle)
{

}

SCENE_DRAW(SceneTitle)
{
	DrawCenterString(200, 200, "Title", GetColor(255, 255, 255), 30);
}