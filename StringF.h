#pragma once


/// <summary>
/// 中央揃えで文字列を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="text">文字列</param>
/// <param name="color">色</param>
/// <param name="size">文字サイズ</param>
void DrawCenterString(int x, int y, const char* text, unsigned int color, int size);
/// <summary>
/// 中央揃えで文字列を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="text">文字列</param>
/// <param name="color">色</param>
/// <param name="size">文字サイズ</param>
void DrawCenterString(float x, float y, const char* text, unsigned int color, float size);

/// <summary>
/// 右揃えで文字列を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="text">文字列</param>
/// <param name="color">色</param>
/// <param name="size">文字サイズ</param>
void DrawRightString(int x, int y, const char* text, unsigned int color, int size);
/// <summary>
/// 右揃えで文字列を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="text">文字列</param>
/// <param name="color">色</param>
/// <param name="size">文字サイズ</param>
void DrawRightString(float x, float y, const char* text, unsigned int color, float size);

/// <summary>
/// 左揃えで文字列を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="text">文字列</param>
/// <param name="color">色</param>
/// <param name="size">文字サイズ</param>
void DrawLeftString(int x, int y, const char* text, unsigned int color, int size);
/// <summary>
/// 左揃えで文字列を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="text">文字列</param>
/// <param name="color">色</param>
/// <param name="size">文字サイズ</param>
void DrawLeftString(float x, float y, const char* text, unsigned int color, float size);

/// <summary>
/// 中央揃えで文字と変数を描画する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="color"色></param>
/// <param name="size">文字サイズ</param>
/// <param name="format">文字列</param>
/// <param name="">変数</param>
void DrawCenterFormat(float x, float y, unsigned int color, float size, const char* format, ...);

