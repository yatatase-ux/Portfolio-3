#pragma once
#include <memory>

class SceneBase;

class  SceneManager
{
	std::unique_ptr<SceneBase> currentState;
	bool isRunning;
	float gameTime;

public:
	SceneManager(std::unique_ptr<SceneBase> initialState);
	void ChangeState(std::unique_ptr<SceneBase> newState);
	void Update(float deltaTime);
};