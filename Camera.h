#pragma once
#include "Stage.h"
#include "FloatXYZ.h"

class Player;

class Camera
{

public:
	Camera();
	~Camera();

	void Update(FloatXYZ playerPos);
};

