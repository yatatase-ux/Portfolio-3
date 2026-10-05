#include "SceneManager.h"
#include "SceneBase.h"

SceneManager::SceneManager(std::unique_ptr<SceneBase> initialState)
	: isRunning(true), gameTime(0.0f), currentState(std::move(initialState))
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

	currentState->Update(this, gameTime);
}