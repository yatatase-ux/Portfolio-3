#include "StringF.h"
#include "DxLib.h"
#include <cstdarg> // va_list関連
#include <cstdio>  // vsprintf_s

void DrawCenterString(int x, int y, const char* text, unsigned int color, int size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	int draw_x = x - GT_w / 2;
	int draw_y = y - size / 2;

	DrawString(draw_x, draw_y, text, color);
}

void DrawCenterString(float x, float y, const char* text, unsigned int color, float size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	float draw_x = x - (float)GT_w / 2.0f;
	float draw_y = y - (float)size / 2.0f;

	DrawStringF(draw_x, draw_y, text, color);
}

void DrawRightString(int x, int y, const char* text, unsigned int color, int size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	int draw_x = x - GT_w;
	int draw_y = y - size / 2;

	DrawString(draw_x, draw_y, text, color);
}

void DrawRightString(float x, float y, const char* text, unsigned int color, float size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	float draw_x = x - (float)GT_w;
	float draw_y = y - (float)size / 2.0f;

	DrawStringF(draw_x, draw_y, text, color);
}

void DrawLeftString(int x, int y, const char* text, unsigned int color, int size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	int draw_x = x;
	int draw_y = y - size / 2;

	DrawString(draw_x, draw_y, text, color);
}

void DrawLeftString(float x, float y, const char* text, unsigned int color, float size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	float draw_x = x;
	float draw_y = y - (float)size / 2.0f;

	DrawStringF(draw_x, draw_y, text, color);
}

void DrawCenterFormat(float x, float y, unsigned int color, float size, const char* format, ...)
{
	char buffer[256]; // 描画する文字列の一時バッファ

	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, sizeof(buffer), format, args); // formatと可変長引数から文字列を組み立てる
	va_end(args);

	SetFontSize(size);
	int GT_s = strlen(buffer);
	int GT_w = GetDrawStringWidth(buffer, GT_s);
	float draw_x = x - (float)GT_w / 2.0f;
	float draw_y = y - size / 2.0f;
	DrawStringF(draw_x, draw_y, buffer, color);
}