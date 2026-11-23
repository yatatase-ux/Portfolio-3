#pragma once
#include <memory>
#include "KeyAction.h"
#include "MouseAction.h"

class SceneBase;

class  SceneManager
{
	std::unique_ptr<SceneBase> currentScene;

	bool isRunning;
	float gameTime;

	KeyAction key;	
	MouseAction mouse;

public:
	SceneManager(std::unique_ptr<SceneBase> initialState);
	~SceneManager();
	void ChangeState(std::unique_ptr<SceneBase> newState);
	void Update(float deltaTime);
	KeyAction& GetKey() { return key; };
	MouseAction& GetMouse() { return mouse; }
};