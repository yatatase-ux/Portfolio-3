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

/// <summary>
/// 指定した四角の中をチェック柄で埋める関数
/// </summary>
/// <param name="leftTop">左上の座標</param>
/// <param name="rightBottom">右下の座標</param>
/// <param name="color1">1つ目の色</param>
/// <param name="color2">2つ目の色</param>
/// <param name="patSize">1つ1つの四角の大きさ</param>
void DrawCheckered(FloatXY leftTop, FloatXY rightBottom,
	unsigned int color1, unsigned int color2, float patSize);
