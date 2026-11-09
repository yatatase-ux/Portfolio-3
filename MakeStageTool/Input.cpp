#include "Input.h"
#include "DxLib.h"
#include "ScreenSize.h"

InputManager::InputManager()
{

}

void InputManager::Input()
{
	int mouseX;
	int mouseY;
	GetMousePoint(&mouseX, &mouseY);
}