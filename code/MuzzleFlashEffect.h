#pragma once
#include"IEffect.h"
#include"nlohmannJson.h"

class MuzzleFlashEffect : public IEffect
{
public:
	MuzzleFlashEffect();
	~MuzzleFlashEffect();

	void init();
	void update();
	void draw();
	void play();
	void stop();
	void setPosition(const VECTOR playingPosition);
	void setRotationAngle(const VECTOR rotation);
	void setScale(const float scale);
	void setSpeed(const float speed);

private:
	int				effectHandle_;
	int				playingHandle_;
	VECTOR			position_;
	nlohmann::json	data_;
};