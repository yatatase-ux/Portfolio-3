#include "SceneTitle.h"
#include "1-SceneInclude.h"
#include "SceneInGame.h"

SCENE_ENTER(SceneTitle)
{

}

SCENE_UPDATE(SceneTitle)
{

	Draw();
	Sound();


	if (key->Push(SPACE))
	{
		ChangeScene(manager);
	}
}

SCENE_EXIT(SceneTitle)
{

}

SCENE_DRAW(SceneTitle)
{
	DrawCenterString(200, 200, "Title", GetColor(255, 255, 255), 30);
}

SCENE_SOUND(SceneTitle)
{

}

SCENE_CHANGE(SceneTitle)
{
	manager->ChangeState(std::make_unique<SceneInGame>());
}