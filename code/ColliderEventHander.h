#pragma once
#include"Singleton.h"

class HitBulletEvent;
class DestroyObjectEvent;
class OnDamageEvent;

class ColliderEventHander : public Singleton<ColliderEventHander>
{
public:
	~ColliderEventHander();

	const std::shared_ptr<HitBulletEvent> getHitBulletEvent()const noexcept { return hitBulletEvent_; }
	const std::shared_ptr<DestroyObjectEvent> getDestroyObjectEvent()const noexcept { return destroyObjectEvent_; }
	const std::shared_ptr<OnDamageEvent> getOnDamageEvent()const noexcept { return onDamageEvent_; }

private:
	friend class Singleton<ColliderEventHander>;
	ColliderEventHander();

	std::shared_ptr<HitBulletEvent> hitBulletEvent_;
	std::shared_ptr<DestroyObjectEvent> destroyObjectEvent_;
	std::shared_ptr<OnDamageEvent> onDamageEvent_;
};