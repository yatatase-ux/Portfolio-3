#pragma once
#include <string>

class SceneManager;
class KeyAction;
class MouseAction;

class SceneBase
{
protected:
public:
	// 派生クラス(EditStage など)のデストラクタも正しく呼ばれるように、仮想にする
	virtual ~SceneBase() = default;
	virtual void Enter(SceneManager* manager) = 0;
	virtual void Input(SceneManager* manager, float deltaTime, KeyAction* key, MouseAction* mouse) = 0;
	virtual void Update(SceneManager* manager, float deltaTime) = 0;
	virtual void Exit(SceneManager* manager) = 0;
	/*　シーンの切り替わり処理（例：フェードイン・フェードアウトなど）を
　　実装するのに非常に使える　*/
	virtual const std::string GetName() const = 0;

	virtual void Draw() = 0;
};

#define SCENE_CLASS(className)\
	void Enter(SceneManager*  manager);\
	void Input(SceneManager* manager, float deltaTime, KeyAction* key, MouseAction* mouse);\
	void Update(SceneManager* manager, float deltaTime);\
	void Exit(SceneManager*   manager);\
	const std::string GetName()const { return #className; };\
	void Draw();

#define SCENE_ENTER(className)\
void className::Enter(SceneManager* manager)

#define SCENE_INPUT(className)\
void className::Input(SceneManager* manager,float deltaTime, KeyAction* key, MouseAction* mouse)

#define SCENE_UPDATE(className)\
void className::Update(SceneManager*  manager, float deltaTime)

#define SCENE_EXIT(className)\
void className::Exit(SceneManager*  manager)

#define SCENE_DRAW(className)\
void className::Draw()