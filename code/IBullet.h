#pragma once

enum class BulletOwnerType
{
	PLAYER,
	ENEMY,
	OTHER
};

class BulletStatus;

class IBullet
{
public:
	virtual ~IBullet() = default;

	virtual void update()		= 0;
	virtual void draw()const	= 0;

	virtual const std::shared_ptr<BulletStatus> getStatus()const noexcept	= 0;
	virtual const BulletOwnerType getBulletOwner()const noexcept			= 0;
};