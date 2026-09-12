#pragma once

class BulletStatus;

class NormalBulletStatus : public BulletStatus
{
public:
	NormalBulletStatus(const float attackPower);
	~NormalBulletStatus();

	void init();
	void use(const VECTOR initPosition, const float initSpeed, const VECTOR initDirection);
	void setPosition();
	void deathpown();

private:

	const int disappearing_time = 90;
};