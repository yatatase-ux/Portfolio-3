#pragma once
#include "Player.h"
#include "Stage.h"
#include "Camera.h"
#include <memory>
#include <string>
#include "KeyAction.h"
#include "GravityDir.h"

// ---- 定数 ----
static constexpr int MAX_SWITCHES = 2;               // 連続で切り替えられる回数
static constexpr int SWITCH_COOLDOWN = 3 * SECOND_RATE; // 切り替え後のクールダウン(3秒)
static constexpr int COMBO_WINDOW = 1 * SECOND_RATE; // 2回目を受け付ける時間(1秒)

class GravityManager
{
	GravityDir gravity;
	KeyAction& key;

	Stage stage;
	std::unique_ptr<Player> player;
	Camera camera;

	int switchCooldown;			// 重力操作のクールダウン
	int remainingSwitches;		// 重力操作の残り回数（最大２連続まで）
	int comboTimer;				// ２連続を受け付ける時間

public:

	GravityManager(const std::string stageNum, KeyAction& arg_key);
	~GravityManager();

	void Input();
	PlayerState Update();
	void Draw();

private:
	bool CanSwitch() const;				// 今、切り替えられるか
	void ApplySwitch(GravityDir next);	// 実際に切り替える

};

