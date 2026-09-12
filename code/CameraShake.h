#pragma once

class CameraShake
{
public:
	CameraShake();
	~CameraShake();

	void init();
	void update(VECTOR& cameraTargetPosition);
	void start(const VECTOR shakeSpeed, const float shakeWidth, const float shakeHeight, const int maxShakeCount);

private:
	void shake(VECTOR& cameraTargetPosition);

	int		shakeCount_;
	int		maxShakeCount_;
	bool	isShaking_;
	VECTOR	currentShakeValue_;
	VECTOR	shakeSpeed_;
	float	shakeWidht_;
	float	shakeHeight_;
	bool	isUpShaking_;
	bool	isRightShaking_;
};