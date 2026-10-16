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

	USE_KEY_MAX
};

class KeyAction
{
protected:

	int key_type[USE_KEY_MAX] =
	{
		KEY_INPUT_TAB,

		KEY_INPUT_SPACE,

		KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT,
		KEY_INPUT_UP, 
		KEY_INPUT_DOWN,

		KEY_INPUT_W,
		KEY_INPUT_A,
		KEY_INPUT_S,
		KEY_INPUT_D
	};

	bool key_sw[USE_KEY_MAX] = { false };

public:

	/// <summary>
	/// 短押し
	/// </summary>
	/// <param name="KeyCode">キー</param>
	/// <returns></returns>
	int Push(int KeyCode)
	{
		if (CheckHitKey(key_type[KeyCode]))
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

	/// <summary>
	/// 長押し
	/// </summary>
	/// <param name="KeyCode">キー</param>
	/// <returns></returns>
	int Check(int KeyCode)
	{
		if (CheckHitKey(key_type[KeyCode]))
		{
			return true;

		}

		return false;
	}

};