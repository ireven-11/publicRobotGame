#include"DxLibForIreven.h"
#include"CameraShake.h"

CameraShake::CameraShake()
{
	init();
}

CameraShake::~CameraShake()
{
}

void CameraShake::init()
{
	isShaking_			= false;
	currentShakeValue_	= zero_vector;
	shakeSpeed_			= zero_vector;
	shakeWidht_			= 0.0f;
	shakeHeight_		= 0.0f;
	shakeCount_			= 0;
	maxShakeCount_		= 0;
	isUpShaking_		= true;
	isRightShaking_		= true;
}

//SetCameraPositionAndTarget_UpVecYを呼ぶ前にこのupdateを呼んでください
void CameraShake::update(VECTOR& cameraTargetPosition)
{
	if (!isShaking_) return;

	++shakeCount_;

	shake(cameraTargetPosition);

	//シェイクを終了する
	if (shakeCount_ >= maxShakeCount_)
	{
		init();
	}
}

void CameraShake::start(const VECTOR shakeSpeed, const float shakeWidth, const float shakeHeight, const int maxShakeCount)
{
	isShaking_		= true;
	shakeSpeed_		= shakeSpeed;
	shakeWidht_		= shakeWidth;
	shakeHeight_	= shakeHeight;
	maxShakeCount_	= maxShakeCount;
}

void CameraShake::shake(VECTOR& cameraTargetPosition)
{
	if (isUpShaking_)
	{
		//シェイクした量を保存しておく
		currentShakeValue_.y += shakeSpeed_.y;

		cameraTargetPosition.y	+= shakeSpeed_.y;

		if (currentShakeValue_.y >= shakeHeight_ * 0.5f)
		{
			isUpShaking_			= false;
			currentShakeValue_.y	= 0.0f;
		}
	}
	else
	{
		//シェイクした量を保存しておく
		currentShakeValue_.y -= shakeSpeed_.y;

		cameraTargetPosition.y	-= shakeSpeed_.y;

		if (currentShakeValue_.y <= -shakeHeight_ * 0.5f)
		{
			isUpShaking_			= true;
			currentShakeValue_.y	= 0.0f;
		}
	}

	if (isRightShaking_)
	{
		//シェイクした量を保存しておく
		currentShakeValue_.x += shakeSpeed_.x;

		cameraTargetPosition.x += shakeSpeed_.x;

		if (currentShakeValue_.x >= shakeWidht_ * 0.5f)
		{
			isRightShaking_			= false;
			currentShakeValue_.x	= 0.0f;
		}
	}
	else
	{
		//シェイクした量を保存しておく
		currentShakeValue_.x -= shakeSpeed_.x;

		cameraTargetPosition.x	-= shakeSpeed_.x;

		if (currentShakeValue_.x <= -shakeWidht_ * 0.5f)
		{
			isRightShaking_			= true;
			currentShakeValue_.x	= 0.0f;
		}
	}
}