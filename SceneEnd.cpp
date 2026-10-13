#include "SceneEnd.h"
#include "1-SceneInclude.h"
#include "SceneTitle.h"

SCENE_ENTER(SceneEnd)
{

}

SCENE_UPDATE(SceneEnd)
{
	if (key->Push(SPACE))
	{
		manager->ChangeState(std::make_unique<SceneTitle>());
	}
}

SCENE_EXIT(SceneEnd)
{

}

SCENE_DRAW(SceneEnd)
{
	DrawCenterString(200, 200, "End", GetColor(255, 255, 255), 30);
}