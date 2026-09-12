#pragma once

class WeaponStatus;

class CannonStatus : public WeaponStatus
{
public:
	CannonStatus();
	~CannonStatus();

	void init();
	void setTargetingPosition(const VECTOR targetPosition, const float screenAttackRangeRadius, const float adustingAttackRange);
	void startFindingTarget();
	void setMuzzlePosition();
	void reinforceFireBulletSpeed();
	void reinforceFireRate();
	void reinforceAttackPower();
	void reinforceAttackRange();
	void reinforceAllStatus();

	const int getMuzzleBoneNumber()const noexcept { return muzzleBoneNumber_; }

private:
	int			muzzleBoneNumber_;
	static int	cannonModel_;
};