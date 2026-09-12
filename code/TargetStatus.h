#pragma once
#include"ITargetableObject.h"

class MapObjectStatus;

class TargetStatus : public MapObjectStatus, public ITargetableObject
{
public:
	TargetStatus() = default;
	TargetStatus(const VECTOR initposition);
	~TargetStatus();

	void init();
	void update();
	void decreaseHp(const float damage);
	void destroy();
	void setOnDamage(const bool isHiting);

	const VECTOR getTargetedPosition()const noexcept { return colliderPosition_; }
	const bool getCanDestroy()const noexcept { return canDestroy_; }
	const bool getOnDamage()const noexcept { return onDamage_; }
	const float	getAdjustingAttackDistance()const noexcept;
	
private:
	VECTOR		colliderPosition_;
	float		hp_;
	bool		canDestroy_;
	static int	targetModel_;
	bool		onDamage_;
	
	const VECTOR	init_position;
	const float		adjust_target_collider_position = 0.75f;
};