#pragma once

class IEffect
{
public:
	~IEffect() = default;

	virtual void init()										= 0;
	virtual void draw()										= 0;
	virtual void play()										= 0;
	virtual void stop()										= 0;
	virtual void update()									= 0;
	virtual void setPosition(const VECTOR playingPosition)	= 0;
	virtual void setRotationAngle(const VECTOR rotation)	= 0;
	virtual void setScale(const float scale)				= 0;
	virtual void setSpeed(const float speed)				= 0;
};