#pragma once
#include "IntXY.h"
#include "FloatXY.h"

/// <summary>
/// 中心を受け取ってBoxを描画する関数
/// </summary>
/// <param name="pos">座標</param>
/// <param name="size">サイズ</param>
/// <param name="color">色</param>
/// <param name="flag">TRUE:Fill  FALSE:Line</param>
/// <param name="LineSize">枠線の太さ</param>
void DrawCentrBox(IntXY pos, IntXY size, unsigned int color, bool flag, int LineSize = 1);
/// <summary>
/// 中心を受け取ってBoxを描画する関数
/// </summary>
/// <param name="pos">座標</param>
/// <param name="size">サイズ</param>
/// <param name="color">色</param>
/// <param name="flag">TRUE:Fill  FALSE:Line</param>
/// <param name="LineSize">枠線の太さ</param>
void DrawCenterBox(FloatXY pos, FloatXY size, unsigned int color, bool flag, float LineSize = 1.0f);


