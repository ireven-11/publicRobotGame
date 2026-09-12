#include"DxLib.h"
#include"RotationEffectCamera.h"

RotationEffectCamera::RotationEffectCamera(const float verticalOffset, const float horizontalOffset,
	const float rotationSpeed, const bool isRightRotation):
	vertical_offset(verticalOffset),
	horizontal_offset(horizontalOffset),
	rotation_speed(rotationSpeed),
	is_right_rotation(isRightRotation)
{
	init();
}

RotationEffectCamera::~RotationEffectCamera()
{
}

void RotationEffectCamera::init()
{
	cameraPosition_			= VGet(0.0f, 0.0f, 0.0f);
	currentRotationValue_	= 0.0f;
}

void RotationEffectCamera::update(const VECTOR targetPosition, const float cameraAngle)
{
	rotation(targetPosition);

	//ÉJÉÅÉâÇÃíçéãì_Çê›íË
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition);
}

void RotationEffectCamera::rotation(const VECTOR targetPosition, const float cameraAngle)
{
	if (is_right_rotation)
	{
		currentRotationValue_ += rotation_speed;
	}
	else
	{
		currentRotationValue_ -= rotation_speed;
	}

	const float radiun = currentRotationValue_ * DX_PI / 180.0;

	const double addAngleX = cos(radiun) * vertical_offset;
	const double addAngleZ = sin(radiun) * vertical_offset;
	
	cameraPosition_.x = targetPosition.x + static_cast<float>(addAngleX);
	cameraPosition_.z = targetPosition.z + static_cast<float>(addAngleZ);
	cameraPosition_.y = targetPosition.y + horizontal_offset;
}