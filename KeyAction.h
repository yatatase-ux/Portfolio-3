#pragma once
#include "DxLib.h"

// ====================================================================
// キー入力
// ====================================================================
enum {
	TAB,

	SPACE,

	LEFT, 
	RIGHT,
	UP,
	DOWN,

	W,
	A,
	S,
	D,

	ONE,
	TWO,
	THREE,

	SHIFT,
	CTRL,

	USE_KEY_MAX
};

class KeyAction
{
protected:

	static constexpr int KEYS_PER_ACTION = 2;

	// 操作ごとに、反応するキーを最大2つ並べる(使わない枠は -1)
	int key_type[USE_KEY_MAX][KEYS_PER_ACTION] =
	{
		{ KEY_INPUT_TAB,    -1 },					// TAB

		{ KEY_INPUT_SPACE,  -1 },					// SPACE

		{ KEY_INPUT_LEFT,   -1 },					// LEFT
		{ KEY_INPUT_RIGHT,  -1 },					// RIGHT
		{ KEY_INPUT_UP,     -1 },					// UP
		{ KEY_INPUT_DOWN,   -1 },					// DOWN

		{ KEY_INPUT_W,      -1 },					// W
		{ KEY_INPUT_A,      -1 },					// A
		{ KEY_INPUT_S,      -1 },					// S
		{ KEY_INPUT_D,      -1 },					// D

		{ KEY_INPUT_1,		-1 },					// 1
		{ KEY_INPUT_2,		-1 },					// 2
		{ KEY_INPUT_3,		-1 },					// 3

		{ KEY_INPUT_LSHIFT, KEY_INPUT_RSHIFT },		// SHIFT(左右どちらでも)
		{ KEY_INPUT_LCONTROL, KEY_INPUT_RCONTROL }	// CTRL（左右どちらでも）
	};


	bool key_sw[USE_KEY_MAX] = { false };

public:

	/// <summary>
	/// 長押し
	/// </summary>
	/// <param name="KeyCode">キー</param>
	/// <returns></returns>
	bool Check(int KeyCode)
	{
		for (int i = 0; i < KEYS_PER_ACTION; i++)
		{
			const int code = key_type[KeyCode][i];
			// -1 は「割り当てなし」なので飛ばす
			if (code != -1 && CheckHitKey(code))
			{
				return true;
			}
		}

		return false;
	}

	/// <summary>
	/// 短押し
	/// </summary>
	/// <param name="KeyCode">キー</param>
	/// <returns></returns>
	bool Push(int KeyCode)
	{
		// どれか1つでも押されているか(左右シフトなどはここでまとめて判定済み)
		if (Check(KeyCode))
		{
			if (key_sw[KeyCode] == false)
			{
				key_sw[KeyCode] = true;
				return true;
			}
		}
		else
		{
			key_sw[KeyCode] = false;
		}


		return false;
	}


};