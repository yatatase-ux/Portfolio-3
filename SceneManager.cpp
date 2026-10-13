#include "SceneManager.h"
#include "SceneBase.h"

SceneManager::SceneManager(std::unique_ptr<SceneBase> initialState)
	:currentState(std::move(initialState)), isRunning(true), gameTime(0.0f)
{
	currentState->Enter(this);
}

void SceneManager::ChangeState(std::unique_ptr<SceneBase> newState)
{
	currentState->Exit(this);
	currentState = std::move(newState);
	currentState->Enter(this);
}

void SceneManager::Update(float deltaTime)
{
	gameTime += deltaTime;

	currentState->Update(this, deltaTime, &key);
	currentState->Draw();
}