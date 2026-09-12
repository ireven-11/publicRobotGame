#pragma once

class BulletStatus;

class ExplosionBulletStatus : public BulletStatus
{
public:
	ExplosionBulletStatus(const float attackPower);
	~ExplosionBulletStatus();

	void init();
	void setPosition();
	void deathpown();
	void use(const VECTOR initPosition, const float initSpeed, const VECTOR initDirection);

private:

	const int disappearing_time = 90;
};