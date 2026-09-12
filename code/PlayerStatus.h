#pragma once
#include"CharaStatus.h"
#include<memory>
#include"ICapsuleCollider.h"
#include"ITargetableObject.h"
#include"WeaponStatus.h"
#include"IWeapon.h"

class IWeapon;
enum class EquipSlot;

class PlayerStatus : public CharaStatus, public ICapsuleCollider, public ITargetableObject
{
public:
	PlayerStatus();
	~PlayerStatus();

	void init();
	void rightRotationModelAngleY();
	void leftRotationModelAngleY();
	void decelerate();
	void addWalkSpeed();
	void addDashSpeed();
	void movePosition(const VECTOR moveDirection);
	void raisePosition();
	void descentPosition();
	void setFacingDirection(const VECTOR cameraPosition, const VECTOR cameraTargetPosition);
	void setColliderPosition();
	void pushBack(const VECTOR pushBackVector);
	void decreaseHp(const float damage);
	void destroy();
	void setOnDamage(const bool isHiting);
	void addExp(const int value);
	void levelUp();

	template<class Weapon>
	void reinforceOrChangeRightWeapon()
	{
		//すでに同じ武器を装備しているなら全ステータス強化
		if (rightWeapon_ == std::dynamic_pointer_cast<Weapon>(rightWeapon_))
		{
			rightWeapon_->getWeaponStatus()->reinforceAllStatus();
		}
		else
		{

			rightWeapon_ = std::make_shared<Weapon>(EquipSlot::RIGHT_HAND);
		}
	}
	template<class Weapon>
	void reinforceOrChangeLeftWeapon()
	{
		//すでに同じ武器を装備しているなら全ステータス強化
		if (leftWeapon_ == std::dynamic_pointer_cast<Weapon>(leftWeapon_))
		{
			leftWeapon_->getWeaponStatus()->reinforceAllStatus();
		}
		else
		{

			leftWeapon_ = std::make_shared<Weapon>(EquipSlot::LEFT_HAND);
		}
	}
	template<class Weapon>
	void reinforceOrChangeShouldertWeapon()
	{
		//すでに同じ武器を装備しているなら全ステータス強化
		if (shoulderWeapon_ == std::dynamic_pointer_cast<Weapon>(shoulderWeapon_))
		{
			shoulderWeapon_->getWeaponStatus()->reinforceAllStatus();
		}
		else
		{

			shoulderWeapon_ = std::make_shared<Weapon>(EquipSlot::RIGHT_SHOULDER);
		}
	}
	
	std::unordered_map<std::string, int> getModelBoneIndex()const noexcept { return modelBoneIndex_; }
	const VECTOR getFacingDirection()const noexcept { return facingDirection_; }
	std::shared_ptr<IWeapon> getLeftWeapon()const noexcept { return leftWeapon_; }
	std::shared_ptr<IWeapon> getRightWeapon()const noexcept { return rightWeapon_; }
	std::shared_ptr<IWeapon> getShoulderWeapon()const noexcept { return shoulderWeapon_; }
	const VECTOR getFacingWeaponPosition()const noexcept { return facingDirection_; }
	const float getScreenAttackRangeRadius()const noexcept { return screenAttackRangeRadius_; }
	const VECTOR getCapsuleBottomPosition()const noexcept { return capusuleColliderBottomPosition_; }
	const VECTOR getCapsuleTopPosition()const noexcept { return capusuleColliderTopPosition_; }
	const float	getCapsuleRadius()const noexcept { return capsuleCollierRadius_; }
	const float	getAdjustingAttackDistance()const noexcept { return getCapsuleRadius(); }
	const VECTOR getTargetedPosition()const noexcept { return VGet(capusuleColliderTopPosition_.x, (capusuleColliderTopPosition_.y + capusuleColliderBottomPosition_.y) * 0.5f, capusuleColliderTopPosition_.z); }
	const bool getCanDestroy()const noexcept { return canDestory_; }
	const VECTOR getRightBoosterPosition()const noexcept { return MV1GetFramePosition(modelHandle_, getModelBoneIndex()["BackBooster_R"]); }
	const VECTOR getLeftBoosterPosition()const noexcept { return MV1GetFramePosition(modelHandle_, getModelBoneIndex()["BackBooster_L"]); }
	const bool getOnDamage()const noexcept { return onDamage_; }
	const float getCurrentExp()const noexcept { return currentExp_; }
	const float getCurrentLevelUpExp()const noexcept { return currentLevelUpExp_; }
	const float	getPrevLevelUpExp()const noexcept { return prevLevelUpExp_; }
	const VECTOR getRightBoosterRotationAngle()const noexcept;
	const VECTOR getLeftBoosterRotationAngle()const noexcept;
	
private:
	std::shared_ptr<IWeapon> leftWeapon_;
	std::shared_ptr<IWeapon> rightWeapon_;
	std::shared_ptr<IWeapon> shoulderWeapon_;
	std::unordered_map<std::string, int> modelBoneIndex_;
	VECTOR	facingDirection_;
	float	screenAttackRangeRadius_;
	VECTOR	capusuleColliderTopPosition_;
	VECTOR	capusuleColliderBottomPosition_;
	float	capsuleCollierRadius_;
	bool	canDestory_;
	bool	onDamage_;
	float	currentExp_;
	int		currentLevel_;
	float	currentLevelUpExp_;
	float	prevLevelUpExp_;

	const float altitude_limit = 350.0f;
};