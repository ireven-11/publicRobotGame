#pragma once
#include<vector>

class IBullet;
class WeaponStatus;

class BombBug : public IWeapon
{
public:
	BombBug(const EquipSlot equipSlot);
	~BombBug();

	void update();
	void draw();
	void setTransform(const MATRIX& equipingMatrix);

	const std::vector<std::shared_ptr<IBullet>> getBullets()const noexcept { return bullets_; }
	const std::shared_ptr<WeaponStatus> getWeaponStatus()const noexcept { return status_; }
	const EquipSlot getCurrentEquipSlot()const noexcept { return currentEquipSlot_; }

private:
	void init();
	void occurEvent();

	std::shared_ptr<WeaponStatus> status_;
	int startTime_;
	EquipSlot currentEquipSlot_;

	std::vector<std::shared_ptr<IBullet>> bullets_;

	const float one_seconds_dxlib = 1000.0f;
};