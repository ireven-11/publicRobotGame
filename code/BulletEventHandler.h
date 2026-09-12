#pragma once
#include"Singleton.h"

class RightFireBulletEvent;
class LeftFireBulletEvent;

class BulletEventHandler : public Singleton<BulletEventHandler>
{
public:
	~BulletEventHandler();

	const std::shared_ptr<RightFireBulletEvent> getRightFireBulletEvent()const noexcept { return rightFireBulletEvent_; }
	const std::shared_ptr<LeftFireBulletEvent> getLeftFireBulletEvent()const noexcept { return leftFireBulletEvent_; }

private:
	friend class Singleton<BulletEventHandler>;
	BulletEventHandler();

	std::shared_ptr<RightFireBulletEvent> rightFireBulletEvent_;
	std::shared_ptr<LeftFireBulletEvent> leftFireBulletEvent_;
};