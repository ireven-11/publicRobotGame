#pragma once
#include"IEffect.h"
#include"nlohmannJson.h"
#include<vector>

struct HitEffectStatus
{
	bool	isUsing;
	int		effectHandle;
	int		playingHandle;
	VECTOR	position;
};

class HitEffect : public IEffect
{
public:
	HitEffect();
	~HitEffect();

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
	void stopFinishingEffect();

	nlohmann::json data_;
	std::vector<HitEffectStatus> statuses_;
};