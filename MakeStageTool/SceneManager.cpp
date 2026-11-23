#include "SceneManager.h"
#include "SceneBase.h"

SceneManager::SceneManager(std::unique_ptr<SceneBase> initialState)
	:currentScene(std::move(initialState)), isRunning(true), gameTime(0.0f)
{
	currentScene->Enter(this);
}

SceneManager::~SceneManager() = default;

void SceneManager::ChangeState(std::unique_ptr<SceneBase> newState)
{
	currentScene->Exit(this);
	currentScene = std::move(newState);
	currentScene->Enter(this);
}

void SceneManager::Update(float deltaTime)
{
	gameTime += deltaTime;

	mouse.Update();

	currentScene->Input(this, deltaTime, &key, &mouse);
	currentScene->Update(this, deltaTime);
	currentScene->Draw();
}