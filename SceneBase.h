#pragma once
#include <string>

class SceneManager;

class SceneBase
{
protected:

public:
	virtual ~SceneBase() = default;
	virtual void Enter(SceneManager* manager) = 0;
	virtual void Update(SceneManager* manager, float deltaTime) = 0;
	virtual void Exit(SceneManager* manager) = 0;
	/*　シーンの切り替わり処理（例：フェードイン・フェードアウトなど）を
　　実装するのに非常に使える　*/
	virtual const std::string GetName() const = 0;
};

#define STATE_CLASS(className)\
	void Enter(SceneManager*  manager);\
	void Update(SceneManager* manager, float deltaTime);\
	void Exit(SceneManager*   manager);\
	const std::string GetName()const { return #className; };