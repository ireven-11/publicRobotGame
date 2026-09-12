#pragma once
#include"NormalCamera.h"
#include"RotationEffectCamera.h"
#include<memory>
#include"PlayerStatus.h"
#include"Timer.h"

constexpr float rotation_effect_offset_horizontal	= 300.0f;
constexpr float rotation_effect_offset_vertical		= 500.0f;
constexpr float rotation_effect_offset_speed		= 1.0f;
constexpr int rotation_effect_offset_seconds		= 5;

class StageStatus;

class CameraBrain
{
public:
	CameraBrain(const std::shared_ptr<PlayerStatus>& status);
	~CameraBrain();

	void init(const std::shared_ptr<PlayerStatus>& status);
	void update(const std::shared_ptr<PlayerStatus>& playerStatus, const std::shared_ptr<StageStatus> stageStatus);
	void postEffect();
	void startEffectCamera();

	const bool getFinishedEffectCamera()const noexcept { return effectTimer_->hasFinishedCountDown(); }
	const bool getFinishedEffectCameraMoment()const noexcept { return effectTimer_->getIsFinishedCountMoment(); }

private:
	void changeCamera(const std::shared_ptr<ICamera>& nextCamera);

	std::shared_ptr<ICamera> currentCamera_;
	std::shared_ptr<Timer> effectTimer_;

	//ÉJÉÅÉâÇÃéÌóﬁ
	static std::shared_ptr<NormalCamera> normalCamera_(const std::shared_ptr<PlayerStatus>& status)
	{
		std::function<void(VECTOR, VECTOR)> setFacingDirection
			= [status](const VECTOR cameraTargetPosition, const VECTOR cameraPosition)
			{
				status->setFacingDirection(cameraTargetPosition, cameraPosition);
			};

		static std::shared_ptr<NormalCamera> camera = std::make_shared<NormalCamera>(setFacingDirection);
		return camera;
	}
	static std::shared_ptr<RotationEffectCamera> rotationEffectCamera_()
	{
		static std::shared_ptr<RotationEffectCamera> camera 
			= std::make_shared<RotationEffectCamera>(rotation_effect_offset_horizontal, rotation_effect_offset_vertical, rotation_effect_offset_speed, true);
		return camera;
	}

	const float	camera_near	= 1.0f;
	const float	camera_far	= 10000.0f;
};