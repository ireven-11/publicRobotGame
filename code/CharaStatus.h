#pragma once
#include"StatusBase3D.h"

class CharaStatus : public StatusBase3D 
{
public:
	virtual ~CharaStatus() = default;

	virtual void init()									= 0;
	virtual void pushBack(const VECTOR pushBackVector)	= 0;

	const float getHp()const noexcept { return hp_; }
	const float getSpeed()const noexcept { return speed_; }
	const float getModelAngleY()const noexcept { return modelAngleY_; }
	
protected:
	float	hp_;
	float	speed_;
	float	modelAngleY_;
};