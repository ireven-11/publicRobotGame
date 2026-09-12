#pragma once
#include<vector>
#include<memory>

enum class EquipSlot
{
	NONE,
	RIGHT_HAND,
	LEFT_HAND,
	RIGHT_SHOULDER
};

class IBullet;
class WeaponStatus;

class IWeapon
{
public:
	virtual ~IWeapon() = default;

	virtual void update()									= 0;
	virtual void draw()										= 0;
	virtual void setTransform(const MATRIX& equipingMatrix)	= 0;

	virtual const std::vector<std::shared_ptr<IBullet>> getBullets()const noexcept	= 0;
	virtual const std::shared_ptr<WeaponStatus> getWeaponStatus()const noexcept		= 0;
	virtual const EquipSlot getCurrentEquipSlot()const noexcept						= 0;
};