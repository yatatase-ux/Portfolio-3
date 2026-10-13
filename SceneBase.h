#pragma once
#include <string>

class SceneManager;
class KeyAction;

class SceneBase
{
protected:
public:
	virtual void Enter(SceneManager* manager) = 0;
	virtual void Update(SceneManager* manager, float deltaTime, KeyAction* key) = 0;
	virtual void Exit(SceneManager* manager) = 0;
	/*　シーンの切り替わり処理（例：フェードイン・フェードアウトなど）を
　　実装するのに非常に使える　*/
	virtual const std::string GetName() const = 0;

	virtual void Draw() = 0;
};

#define SCENE_CLASS(className)\
	void Enter(SceneManager*  manager);\
	void Update(SceneManager* manager, float deltaTime, KeyAction* key);\
	void Exit(SceneManager*   manager);\
	const std::string GetName()const { return #className; };\
	void Draw();\

#define SCENE_ENTER(className)\
void className::Enter(SceneManager*  manager)

#define SCENE_UPDATE(className)\
void className::Update(SceneManager*  manager, float deltaTime, KeyAction* key)

#define SCENE_EXIT(className)\
void className::Exit(SceneManager*  manager)

#define SCENE_DRAW(className)\
void className::Draw()