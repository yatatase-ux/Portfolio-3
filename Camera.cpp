#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
	offsetX = 0.0f;
}

Camera::~Camera()
{

}

FloatXY Camera::WorldToScreen(FloatXY worldPos) const
{
	return worldPos - FloatXY{ offsetX, 0.0f };
}

void Camera::Follow(FloatXYZ p_wPos, FloatXY stageSize)
{
	offsetX = p_wPos.x - 640;
	offsetX = std::clamp(offsetX, 0.0f, stageSize.x - 1280);
}