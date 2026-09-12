#pragma once
#include"ICapsuleCollider.h"

class EnemyStatus;

class DroneStatus : public EnemyStatus, public ICapsuleCollider
{
public:
	DroneStatus() = default;
	DroneStatus(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destinations);
	~DroneStatus();

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

	const VECTOR	getCapsuleBottomPosition()const noexcept { return colliderBottomPosition_; }
	const VECTOR	getCapsuleTopPosition()const noexcept { return colliderTopPosition_; }
	const float		getCapsuleRadius()const noexcept { return colliderRadius_; }
	const float		getAdjustingAttackDistance()const noexcept { return getCapsuleRadius(); }
	const VECTOR	getTargetedPosition()const noexcept { return VAdd(position_, VGet(0.0f, colliderRadius_ * 0.5f, 0.0f)); }
	const VECTOR	getMuzzleDirection(const VECTOR adjustMuzzleDirection = VGet(0.0f, 0.0f, 0.0f))const noexcept;

private:

	static int	droneModel_;
	float		colliderRadius_;
	VECTOR		moveDirection_;
	VECTOR		colliderTopPosition_;
	VECTOR		colliderBottomPosition_;
	float		modelRotationTimeY_;
	float		modelRotationTimeX_;
	float		modelAngleX_;

	const VECTOR	tower_position;
	const float		turn_around_speed		= 0.2f;
	const float		chace_target_between_y	= 2.5f;
	const float		distance_error			= 5.0f;
};