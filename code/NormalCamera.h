#pragma once
#include"ICamera.h"
#include<functional>
#include<memory>

class Player;
class PostProcessing;
class CameraShake;

class NormalCamera : public ICamera
{
public:
	NormalCamera(const std::function<void(VECTOR, VECTOR)>& playerFunction);
	~NormalCamera();

	void init();
	void update(const VECTOR playerPosition, const float cameraAngle = 0.0f);

	const VECTOR getTargetPosition()const noexcept { return targetPosition_; }
	const std::shared_ptr<PostProcessing> getPostProcessing()const noexcept { return currentPostEffect_; }

private:
	void rotation(const VECTOR playerPosition, const float cameraAngle = 0.0f);
	void leftBehind();
	void eventRegister();

	VECTOR	position_;
	VECTOR	targetPosition_;
	float	verticalAngle_;
	bool	isLeavingBehind_;
	float	leavingTime_;
	VECTOR	currentOffset_;
	VECTOR	currentTargetOffset_;
	std::function<void(VECTOR, VECTOR)> giveCameraPosition_;
	std::shared_ptr<PostProcessing> currentPostEffect_;
	std::shared_ptr<CameraShake> cameraShake_;

	static std::shared_ptr<PostProcessing> dashPostEffect_()
	{
		static std::shared_ptr<PostProcessing> postEffect = std::make_shared<PostProcessing>("graph/concentrationLine.png");
		return postEffect;
	}
	static std::shared_ptr<PostProcessing> damagePostEffect_()
	{
		static std::shared_ptr<PostProcessing> postEffect = std::make_shared<PostProcessing>("graph/damagePostProcess.png");
		return postEffect;
	}

	const VECTOR	position_offset			= VGet(9.0f, 8.0f, -18.0f);
	const VECTOR	target_position_offset	= VAdd(position_offset, VGet(-1.0f, 1.0f, 40.0f));
	const float		leaving_behind_offset	= position_offset.z * 0.5f;
	const float		rotation_speed			= 0.03f;
	const float		adjust_radiun			= 2.0f;
	const float		max_verticalAngle		= 0.75f;
	const float		min_verticalAngle		= -0.75f;
	const float		max_leaving_time		= 1.0f;
	const float		add_leaving_time		= 0.04f;
	const VECTOR	shake_speed				= VGet(0.1f, 0.1f, 0.0f);
	const float		shake_widht				= 1.0f;
	const float		shake_height			= 1.0f;
	const int		max_shake_count			= 25;
};