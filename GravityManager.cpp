#include "GravityManager.h"
#include "Shape.h"
#include <algorithm>

GravityManager::GravityManager(const std::string stageNum, KeyAction& arg_key, GravityDir sGra)
	:key(arg_key), gravity(sGra)
{
	isChangeGravity = false;

	switchCooldown = 0;
	remainingSwitches = 0;
	comboTimer = 0;
}

GravityManager::~GravityManager()
{

}

bool GravityManager::Input()
{
	// ---- どの重力に切り替えたいか ----
	// Push は毎フレーム呼んでおく(呼ばないフレームがあると、押した判定がずれるため)
	GravityDir next = gravity;
	bool isChangeGra = false;

	if (key.Push(ONE))   next = GravityDir::Down;
	else if (key.Push(TWO))   next = GravityDir::Up;
	else if (key.Push(THREE)) next = GravityDir::Back;

	// 今と違う重力が選ばれて、切り替えられる状態なら切り替える
	if (next != gravity && CanSwitch())
	{
		ApplySwitch(next);
		return true;
	}

	return false;
}

void GravityManager::Update()
{
	// タイマーを減らす(0 で止める)
	if (switchCooldown > 0) switchCooldown--;
	if (comboTimer > 0)     comboTimer--;

}

void GravityManager::Draw()
{
	DrawFormatString(20, 20, GetColor(244, 255, 255), "%d", switchCooldown);
	DrawFormatString(100, 20, GetColor(244, 255, 255), "%d", remainingSwitches);

}

bool GravityManager::CanSwitch() const
{
	if (switchCooldown <= 0) return true;

	return (remainingSwitches > 0 && comboTimer > 0);
}

void GravityManager::ApplySwitch(GravityDir next)
{
	// 休み明け(クールダウン終了後)の1回目なら、回数を満タンに戻してから使う
	if (switchCooldown <= 0)
	{
		remainingSwitches = MAX_SWITCHES;
	}
	remainingSwitches--;

	// 重力を切り替える
	gravity = next;

	// 使うたびにクールダウンは3秒から(2回目ならリセットされる)
	switchCooldown = SWITCH_COOLDOWN;

	// まだ回数が残っていれば、2回目の受付を1秒だけ開ける
	// 使い切ったら受付は閉じる(あとはクールダウンが終わるのを待つ)
	comboTimer = (remainingSwitches > 0) ? COMBO_WINDOW : 0;
}