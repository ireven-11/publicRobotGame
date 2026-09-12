#pragma once
#include"ISphereCollider.h"

class EnemyStatus;

class TankStatus : public EnemyStatus, public ISphereCollider
{
public:
	TankStatus() = default;
	TankStatus(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destinations);
	~TankStatus();

	void init();
	void pushBack(const VECTOR pushBackVector);
	void decreaseHp(const float damage);
	void destroy();
	void moveToFacingDirection();
	void findTarget(const std::shared_ptr<ITargetableObject>& target);
	void resetTarget();
	void rotationToDestinationDirection();
	void setAimingPosition();
	void setColliderPosition();
	void setIsRayObstructed(const bool isOverlap);
	void setMuzzlePosition();
	void shiftToSide();
	void faceToTarget();
	void setHasClearShotToTarget(const bool hasClearShotToTarget);
	void moveNextDestination();
	void setOnDamage(const bool isHiting);
	
	const VECTOR	getSpherePosition()const noexcept { return position_; }
	const float		getRadius()const noexcept { return colliderRadius_; }
	const float		getAdjustingAttackDistance()const noexcept { return getRadius(); }
	const VECTOR	getTargetedPosition()const noexcept { return VAdd(position_, VGet(0.0f, getRadius() * 0.5f, 0.0f)); }
	const VECTOR	getMuzzleDirection(const VECTOR adjustMuzzleDirection = VGet(0.0f, 0.0f, 0.0f))const noexcept;
	
private:

	static int	tankModel_;
	float		colliderRadius_;
	VECTOR		moveDirection_;

	const VECTOR	tower_position;
	const float		distance_error = 10.0f;
};