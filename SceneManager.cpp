#include "SceneManager.h"
#include "SceneBase.h"

SceneManager::SceneManager(std::unique_ptr<SceneBase> initialState)
	:currentScene(std::move(initialState)), isRunning(true), gameTime(0.0f)
{
	currentScene->Enter(this);
}

void SceneManager::ChangeState(std::unique_ptr<SceneBase> newState)
{
	currentScene->Exit(this);
	currentScene = std::move(newState);
	currentScene->Enter(this);
}

void SceneManager::Update(float deltaTime)
{
	gameTime += deltaTime;

	currentScene->Update(this, deltaTime, &key);
	currentScene->Draw();
}