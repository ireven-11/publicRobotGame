#pragma once
#include"ITargetableObject.h"
#include<functional>

class MapObjectStatus;

class TowerStatus : public MapObjectStatus, public ITargetableObject
{
public:
	TowerStatus() = default;
	TowerStatus(const VECTOR initPosition, const std::function<void()>& hpThreeQuarters, const std::function<void()>& hpOneHalf, const std::function<void()>& hpOneQuarters);
	~TowerStatus();

	void init();
	void update();
	void decreaseHp(const float damage);
	void destroy();
	void setOnDamage(const bool isHiting);

	const VECTOR getTargetedPosition()const noexcept { return colliderPosition_; }
	const bool getCanDestroy()const noexcept { return canDestroy_; }
	const bool getOnDamage()const noexcept { return onDamage_; }
	const float	getAdjustingAttackDistance()const noexcept;
	const float getHp()const noexcept { return hp_; }

private:
	VECTOR		colliderPosition_;
	float		hp_;
	bool		canDestroy_;
	static int	TowerModel_;
	bool		onDamage_;
	bool		isHpThreeQuartersMoment_;
	bool		isHpOneHalfMoment_;
	bool		isHpOneQuartersMoment_;

	std::function<void()> hpThreeQuartersEvent_;
	std::function<void()> hpOneHalfEvent_;
	std::function<void()> hpOneQuartersEvent_;

	const VECTOR	init_position;
	const float		adjust_Tower_collider_position = 0.75f;
};