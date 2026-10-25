#pragma once
#include "Stage.h"
#include "FloatXY.h"
#include "FloatXYZ.h"
#include "ScreenSize.h"

class Player;

class Camera
{

	float offsetX;

public:
	Camera();
	~Camera();

	FloatXY WorldToScreen(FloatXY worldPos) const;

	void Follow(FloatXYZ p_wPos, FloatXY stageSize);
};

