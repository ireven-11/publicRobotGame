#pragma once
#include"IEffect.h"
#include"nlohmannJson.h"
#include<vector>

struct ExplosionEffectStatus
{
	bool	isUsing;
	int		effectHandle;
	int		playingHandle;
	VECTOR	position;
};

class ExplosionEffect : public IEffect
{
public:
	ExplosionEffect();
	~ExplosionEffect();

	void init();
	void update();
	void draw();
	void play();
	void stop();
	void setPosition(const VECTOR playingPosition);
	void setRotationAngle(const VECTOR rotation);
	void setScale(const float scale);
	void setSpeed(const float speed);
	void setDefualtScale();

private:
	void stopFinishingEffect();

	float modelNormalScale_;
	nlohmann::json data_;
	std::vector<ExplosionEffectStatus> statuses_;
};