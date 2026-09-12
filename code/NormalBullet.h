#pragma once

class IBullet;
class BulletStatus;
enum class BulletOwnerType;

class NormalBullet : public IBullet
{
public:
	NormalBullet(const float attackPower, const BulletOwnerType owner);
	~NormalBullet();

	void update();
	void draw()const;

	const std::shared_ptr<BulletStatus> getStatus()const noexcept { return status_; }
	const BulletOwnerType getBulletOwner()const noexcept { return bullet_owner; }

private:
	std::shared_ptr<BulletStatus> status_;

	const BulletOwnerType bullet_owner;
};