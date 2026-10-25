#include "SceneEnd.h"
#include "1-SceneInclude.h"
#include "SceneTitle.h"

SceneEnd::SceneEnd(PlayerState r)
	:result(r)
{

}

SCENE_ENTER(SceneEnd)
{

}

SCENE_UPDATE(SceneEnd)
{
	if (key->Push(TAB))
	{
		manager->ChangeState(std::make_unique<SceneTitle>());
	}
}

SCENE_EXIT(SceneEnd)
{

}

SCENE_DRAW(SceneEnd)
{
	switch (result)
	{
	case PlayerState::Clear:
		DrawCenterString(SCREEN_CENTER_W, SCREEN_CENTER_H, "!!CLEAR!!", GetColor(255, 255, 0), 100);
		break;

	case PlayerState::Dead:
		DrawCenterString(SCREEN_CENTER_W, SCREEN_CENTER_H, "FAILED...", GetColor(0, 0, 255), 100);
		break;
	}
}