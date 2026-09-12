#pragma once

class ITargetableObject
{
public:
	virtual ~ITargetableObject() = default;

	virtual void decreaseHp(const float damage) = 0;
	virtual void destroy()						= 0;
	virtual void setOnDamage(const bool isHiting)	= 0;

	virtual const VECTOR	getTargetedPosition()const noexcept			= 0;
	virtual const bool		getCanDestroy()const noexcept				= 0;
	virtual const float		getAdjustingAttackDistance()const noexcept	= 0;
	virtual const bool		getOnDamage()const noexcept					= 0;
};