#pragma once
#include"StatusBase3D.h"

class WeaponStatus : public StatusBase3D
{
public:
	virtual ~WeaponStatus() = default;

	virtual void init() = 0;
	virtual void setTargetingPosition(const VECTOR targetPositionconst, const float screenAttackRangeRadius, const float adustingAttackRange) = 0;
	virtual void startFindingTarget()		= 0;
	virtual void setMuzzlePosition()		= 0;
	virtual void reinforceFireBulletSpeed() = 0;
	virtual void reinforceFireRate()		= 0;
	virtual void reinforceAttackPower()		= 0;
	virtual void reinforceAttackRange()		= 0;
	virtual void reinforceAllStatus()		= 0;

	const VECTOR getCurrentTargetingPosition()const noexcept { return currentTargetingPosition_; }
	const bool getIsFindingTarget()const noexcept { return isFindingTarget_; }
	const VECTOR getMuzzlePosition()const noexcept { return muzzlePosition_; }
	const float getFireBulletSpeed()const noexcept { return fireBulletSpeed_; }
	const float getFireRate()const noexcept { return fireRate_; }
	const float getAttackPower()const noexcept { return attackPower_; }
	const float getAttackRange()const noexcept { return attackRange_; }

protected:
	VECTOR	currentTargetingPosition_;
	bool	isFindingTarget_;
	VECTOR	muzzlePosition_;
	float fireBulletSpeed_;
	float fireRate_;
	float attackPower_;
	float attackRange_;

	const float init_fire_target_offset = 200.0f;
};