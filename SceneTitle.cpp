#include "SceneTitle.h"
#include <iostream>
#include <memory>
#include "DxLib.h"
#include "StringF.h"

void SceneTitle::Enter(SceneManager* manager)
{

}

void SceneTitle::Update(SceneManager* manager, float deltaTime)
{

	DrawCenterString(200, 200, "Title", GetColor(255, 255, 255), 30);

	DrawRightString(200.0f, 200.0f, "Title", GetColor(255, 0, 0), 30.0f);
	DrawLeftString(200.0f, 200.0f, "Title", GetColor(0, 0, 255), 30.0f);

	DrawLine(200, 200 - 15, 200, 215, GetColor(255, 255, 0), 2);
}

void SceneTitle::Exit(SceneManager* manager)
{

}