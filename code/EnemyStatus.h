#pragma once
#include"CharaStatus.h"
#include"ITargetableObject.h"

class EnemyStatus : public CharaStatus, public ITargetableObject
{
public:
	EnemyStatus(const VECTOR initPosition):init_position(initPosition){}
	virtual ~EnemyStatus() = default;

	virtual void init()															= 0;
	virtual void pushBack(const VECTOR pushBackVector)							= 0;
	virtual void decreaseHp(const float damage)									= 0;
	virtual void destroy()														= 0;
	virtual void moveToFacingDirection()										= 0;
	virtual void findTarget(const std::shared_ptr<ITargetableObject>& target)	= 0;
	virtual void resetTarget()													= 0;
	virtual void rotationToDestinationDirection()								= 0;
	virtual void setAimingPosition()											= 0;
	virtual void setColliderPosition()											= 0;
	virtual void setIsRayObstructed(const bool isOverlap)						= 0;
	virtual void setMuzzlePosition()											= 0;
	virtual void shiftToSide()													= 0;
	virtual void faceToTarget()													= 0;
	virtual void setHasClearShotToTarget(const bool hasClearShotToTarget)		= 0;
	virtual void moveNextDestination()											= 0;
	virtual void setOnDamage(const bool isHiting)								= 0;
	
	const bool		getCanDestroy()const noexcept { return canDestroy_; }
	const bool		getIsRayObstructed()const noexcept { return isRayObstructed_; }
	const VECTOR	getMuzzlePosition()const noexcept { return muzzlePosition_; }
	const VECTOR	getAimingPosition()const noexcept { return aimingPosition_; }
	const auto		getMovingDestnationPosition()const noexcept { return movingDestinationPosition_; }
	const bool		getHasClearShotToTarget()const noexcept { return hasClearShotToTarget_; }
	const VECTOR	getFacingPosition()const noexcept { return facingPosition; }
	const bool		getIsTargetInAttackRange()const noexcept { return isTargetInAttackRange_; }
	const bool		getOnDamage()const noexcept { return onDamage_; }
	const float		getDropingExp()const noexcept { return dropingExp_; }

	virtual const VECTOR	getTargetedPosition()const noexcept																= 0;
	virtual const float		getAdjustingAttackDistance()const noexcept														= 0;
	virtual const VECTOR	getMuzzleDirection(const VECTOR adjustMuzzleDirection = VGet(0.0f, 0.0f, 0.0f))const noexcept	= 0;

protected:

	bool				canDestroy_;
	VECTOR				aimingPosition_;
	bool				isRayObstructed_;
	bool				hasClearShotToTarget_;
	VECTOR				muzzlePosition_;
	std::vector<VECTOR>	movingDestinationPosition_;
	VECTOR				facingPosition;
	bool				isTargetInAttackRange_;
	short				movingDestinationIndex_;
	bool				onDamage_;
	float				dropingExp_;
	
	const VECTOR init_position;
};