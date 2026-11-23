#pragma once
#include "SceneBase.h"
#include "FloatXY.h"

class SelectStage : public SceneBase
{
	int stageCount;
	float baseY;
	const FloatXY boxSize = { 1200.0f, 150.0f };
	const float boxSpace = 50.0f;

	int selectStage;

public:

	SelectStage();

	SCENE_CLASS(SelectStage);

private:

	// マウススクロール処理
	void MouseScroll();

	FloatXY IndexToBoxPos(int i);
};

