#pragma once
#include <memory>
#include "KeyAction.h"

class SceneBase;

class  SceneManager
{
	std::unique_ptr<SceneBase> currentState;
	bool isRunning;
	float gameTime;
	KeyAction key;

public:
	SceneManager(std::unique_ptr<SceneBase> initialState);
	void ChangeState(std::unique_ptr<SceneBase> newState);
	void Update(float deltaTime);
	KeyAction& GetKey() { return key; };
};