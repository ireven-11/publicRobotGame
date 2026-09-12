#pragma once

class IBullet;
class BulletStatus;
class Explosion;

class ExplosionBullet : public IBullet
{
public:
	ExplosionBullet(const float attackPower, const float explosionRaidus, const BulletOwnerType owner);
	~ExplosionBullet();

	void update();
	void draw()const;

	const std::shared_ptr<BulletStatus> getStatus()const noexcept { return status_; }
	const std::shared_ptr<Explosion> getExplosion()const noexcept { return explosion_; }
	const BulletOwnerType getBulletOwner()const noexcept { return bullet_owner; }

private:
	std::shared_ptr<BulletStatus> status_;
	std::shared_ptr<Explosion> explosion_;

	const BulletOwnerType bullet_owner;
};