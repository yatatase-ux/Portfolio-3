#include "Camera.h"
#include <algorithm>
#include <cmath>   // std::round

Camera::Camera()
{
	offsetX = 0.0f;
}

Camera::~Camera()
{

}

FloatXY Camera::WorldToScreen(FloatXY worldPos) const
{
	return { worldPos.x - std::round(offsetX), worldPos.y };
}

void Camera::Follow(FloatXYZ p_wPos, FloatXY stageSize)
{
	offsetX = p_wPos.x - 640.0f;
	offsetX = std::clamp(offsetX, 0.0f, stageSize.x - 1280.0f);
}