#pragma once
#include"StatusBase3D.h"

constexpr float bullet_collision_radius = 0.2f;

class BulletStatus : public StatusBase3D
{
public:
	BulletStatus(const float attackPower) : attack_power(attackPower) {}
	virtual ~BulletStatus() = default;

	virtual void init()																				= 0;
	virtual void setPosition()																		= 0;
	virtual void deathpown()																		= 0;
	virtual void use(const VECTOR initPosition, const float initSpeed, const VECTOR initDirection)	= 0;

	const bool		getIsUsing()const noexcept { return isUsing_; }
	const VECTOR	getPrevPosition_()const noexcept { return prevPosition_; }

	const float attack_power;

protected:

	float	currentSpeed_;
	bool	isUsing_;
	VECTOR	moveVector_;
	int		disappearingCounter_;
	VECTOR	moveDirection_;
	VECTOR	prevPosition_;
};