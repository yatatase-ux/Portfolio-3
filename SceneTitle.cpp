#include "SceneTitle.h"
#include "1-SceneInclude.h"
#include "SceneMainMenu.h"

SCENE_ENTER(SceneTitle)
{

}

SCENE_UPDATE(SceneTitle)
{
	if (key->Push(TAB))
	{
		manager->ChangeState(std::make_unique<SceneMainMenu>());
	}
}

SCENE_DRAW(SceneTitle)
{
	DrawCenterString(200, 200, "Title", GetColor(255, 255, 255), 30);
}

SCENE_EXIT(SceneTitle)
{

}
