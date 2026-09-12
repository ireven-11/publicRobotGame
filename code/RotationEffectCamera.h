#pragma once
#include"ICamera.h"

class RotationEffectCamera : public ICamera
{
public:
	RotationEffectCamera() = default;
	RotationEffectCamera(const float verticalOffset, const float horizontalOffset, const float rotationSpeed, const bool isRightRotation);
	~RotationEffectCamera();

	void init();
	void update(const VECTOR targetPostion, const float cameraAngle = 0.0f);

	const std::shared_ptr<PostProcessing> getPostProcessing()const noexcept { return nullptr; }

private:
	void rotation(const VECTOR targetPostion, const float cameraAngle = 0.0f);

	VECTOR cameraPosition_;
	float currentRotationValue_;

	const float vertical_offset;
	const float horizontal_offset;
	const float	rotation_speed;
	const bool is_right_rotation;
};