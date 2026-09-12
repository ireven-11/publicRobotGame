#pragma once
#include"IEffect.h"
#include"nlohmannJson.h"

class EngineEffect : public IEffect
{
public:
	EngineEffect();
	~EngineEffect();

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
	void changeColor();

	int				effectHandle_;
	int				playingHandle_;
	VECTOR			position_;
	nlohmann::json	data_;
	VECTOR			rotation_;
};