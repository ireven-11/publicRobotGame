#pragma once

class WeaponStatus;

class BombBugStatus : public WeaponStatus
{
public:
	BombBugStatus();
	~BombBugStatus();

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
	static int	bombBugModel_;
};